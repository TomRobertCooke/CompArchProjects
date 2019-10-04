#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("error\n");
        return 0;
    }

    unsigned short num;
    if (sscanf(argv[1], "%hu", &num) <= 0)
    {
        printf("bad input\n");
        return 0;
    }

    unsigned short first = num, second = num, i;
    for (i = 0; i < 8; i++)
    {
        if ((first & 1) != ((second & 32768) >> 15))
        {
            printf("Not-Palindrome\n");
            return 0;
        }
        first = first >> 1;
        second = second << 1;
    }

    printf("Is-Palindrome\n");
    return 0;
}

//to check for palindrone, just use the & operator with 32768 and 1, 
//if the numbers aren't equal at any point, then just print and return
//1 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768
