#define _XOPEN_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    printf("%s", crypt("qrst", "50"));
}