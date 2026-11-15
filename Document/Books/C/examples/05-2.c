/* Original teaching example; not compiled or executed here. */
#include <stdio.h>
#include <string.h>

int main(void)
{
    char name[32];
    if (fgets(name, sizeof name, stdin) == NULL) {
        fputs("no input or read error\n", stderr);
        return 1;
    }
    size_t end = strcspn(name, "\n");
    if (name[end] != '\n') {
        fputs("line needs a newline within 31 bytes\n", stderr);
        return 1;
    }
    name[end] = '\0';
    printf("Hello, %s!\n", name);
    return 0;
}
