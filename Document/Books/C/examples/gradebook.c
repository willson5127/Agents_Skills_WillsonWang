/* Original C17 teaching example. Not compiled or executed in this environment. */
#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_RECORDS 1000000UL

typedef struct {
    int *data;
    size_t len;
    size_t cap;
} Scores;

enum ReadStatus { READ_END, READ_OK, READ_INVALID, READ_ERROR };

/* Consume one whole physical line. Capacity includes the terminating zero.
   Reject embedded zero and excessive length; accept a final line without LF. */
static enum ReadStatus read_line(FILE *in, char *buffer, size_t cap)
{
    size_t used = 0;
    int bad = 0;
    int ch;
    while ((ch = fgetc(in)) != EOF && ch != '\n') {
        if (ch == '\0') {
            bad = 1;
        }
        if (used < cap - 1) {
            buffer[used++] = (char)ch;
        } else {
            bad = 1;
        }
    }
    if (ferror(in)) {
        return READ_ERROR;
    }
    if (ch == EOF && used == 0 && !bad) {
        return READ_END;
    }
    buffer[used] = '\0';
    return bad ? READ_INVALID : READ_OK;
}

static int parse_score(const char *text, int *out)
{
    char *end;
    errno = 0;
    long value = strtol(text, &end, 10);
    if (end == text || errno == ERANGE) {
        return 0;
    }
    while (isspace((unsigned char)*end)) {
        ++end;
    }
    if (*end != '\0' || value < 0 || value > 100) {
        return 0;
    }
    *out = (int)value;
    return 1;
}

/* On failure, the logical contents and ownership remain unchanged. */
static int append_score(Scores *s, int value)
{
    if (s->len >= MAX_RECORDS) {
        return 0;
    }
    if (s->len == s->cap) {
        size_t next_cap = 16;
        if (s->cap != 0) {
            if (s->cap > SIZE_MAX / 2) {
                return 0;
            }
            next_cap = s->cap * 2;
        }
        if (next_cap > SIZE_MAX / sizeof *s->data) {
            return 0;
        }
        int *next = realloc(s->data, next_cap * sizeof *s->data);
        if (next == NULL) {
            return 0;
        }
        s->data = next;
        s->cap = next_cap;
    }
    s->data[s->len++] = value;
    return 1;
}

static int compare_score(const void *left, const void *right)
{
    int a = *(const int *)left;
    int b = *(const int *)right;
    return (a > b) - (a < b);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("usage: gradebook.exe FILE\n", stderr);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("open input");
        return 1;
    }

    Scores scores = {NULL, 0, 0};
    char line[128];
    size_t line_number = 0;
    int status = 0;
    for (;;) {
        enum ReadStatus r = read_line(in, line, sizeof line);
        if (r == READ_END) {
            break;
        }
        if (r == READ_ERROR) {
            fputs("input read failed\n", stderr);
            status = 1;
            break;
        }
        /* At most MAX_RECORDS valid lines are accepted before stopping. */
        if (line_number == SIZE_MAX) {
            fputs("too many input lines\n", stderr);
            status = 1;
            break;
        }
        ++line_number;
        int score;
        if (r == READ_INVALID || !parse_score(line, &score)) {
            fprintf(stderr, "invalid score at line %zu\n", line_number);
            status = 1;
            break;
        }
        if (!append_score(&scores, score)) {
            fputs("record limit or allocation failure\n", stderr);
            status = 1;
            break;
        }
    }
    if (fclose(in) == EOF) {
        fputs("input close failed\n", stderr);
        status = 1;
    }

    if (status == 0) {
        if (scores.len == 0) {
            if (puts("count=0 (no scores)") == EOF) {
                status = 1;
            }
        } else {
            unsigned long long total = 0;
            size_t passes = 0;
            for (size_t i = 0; i < scores.len; ++i) {
                total += (unsigned int)scores.data[i];
                if (scores.data[i] >= 60) {
                    ++passes;
                }
            }
            qsort(scores.data, scores.len, sizeof *scores.data, compare_score);
            size_t mid = scores.len / 2;
            double median = scores.data[mid];
            if (scores.len % 2 == 0) {
                median = (scores.data[mid - 1] + scores.data[mid]) / 2.0;
            }
            if (printf("count=%zu min=%d max=%d mean=%.2f median=%.2f passes=%zu\n",
                       scores.len, scores.data[0], scores.data[scores.len - 1],
                       (double)total / scores.len, median, passes) < 0) {
                status = 1;
            }
        }
        if (fflush(stdout) == EOF) {
            status = 1;
        }
    }
    free(scores.data);
    return status;
}
