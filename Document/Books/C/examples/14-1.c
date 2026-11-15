/* Original teaching example; not compiled or executed here. */
#include <stdio.h>
#include <string.h>

int main(void)
{
    unsigned int value = 1;
    unsigned char bytes[sizeof value];
    memcpy(bytes, &value, sizeof value);
    for (size_t i = 0; i < sizeof bytes; ++i) {
        printf("%02X%s", (unsigned int)bytes[i],
               i + 1 == sizeof bytes ? "\n" : " ");
    }
    return 0;
}
