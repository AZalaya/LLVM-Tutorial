#include <stdio.h>

int main(void)
{
    int a = 0;
    int b = 1;
    int c = 2;
    int d = 0;

    if (a < b) {
        c = -1;
    }

    if (c > 0) {
        a = a + 1;
        b = b + 1;
        d = a + b;
    }
    else {
        d = c;
    }

    printf("Final value: %d\n", d);

    return 0;
}