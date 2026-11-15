/* Original teaching example; not compiled or executed here. */
#include <stdio.h>

typedef struct {
    char name[16];
    int score;
} Student;

static void print_student(const Student *s)
{
    printf("%s: %d\n", s->name, s->score);
}

int main(void)
{
    Student first = {.name = "Ada", .score = 90};
    Student copy = first;
    copy.score = 85;
    print_student(&first);
    print_student(&copy);
    return 0;
}
