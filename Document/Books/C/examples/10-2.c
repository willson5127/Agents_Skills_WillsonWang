/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fputs("usage: lines.exe FILE\n", stderr);
        return 1;
    }
    FILE *in = fopen(argv[1], "r");
    if (in == NULL) {
        perror("open");
        return 1;
    }
    char block[128];
    int failed = 0;
    while (fgets(block, sizeof block, in) != NULL) {
        if (fputs(block, stdout) == EOF) {
            failed = 1;
            break;
        }
    }
    if (ferror(in)) {
        failed = 1;
    }
    if (fclose(in) == EOF) {
        failed = 1;
    }
    if (fflush(stdout) == EOF) {
        failed = 1;
    }
    return failed ? 1 : 0;
}
