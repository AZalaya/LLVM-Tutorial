#include "stdio.h"

int fibonacci(int x)
{
    if (x < 3) {
        return 1;
    }
    else {
        return fibonacci(x - 1) + fibonacci(x - 2);
    }
}

int main(void)
{
    int num = 40;
    int result;

    result = fibonacci(num);

    printf("Fibonacci number %d: %d", num, result);

    

    return 0;
}