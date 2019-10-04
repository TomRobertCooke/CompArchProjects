#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
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

    unsigned short checker = num;
    unsigned short count = 0;
    unsigned short cur = 0;
    unsigned short pairs = 0;
    while (checker != 0)
    {
        if ((checker & 1) == 1)
        {
            count++;
            cur++;
			if (cur == 2)
            {
                pairs++;
				cur = 0;
            }
        } else
        {
            cur = 0;
        }
        checker = checker >> 1;
    }

    if ((count & 1) == 1)
    {
        printf("Odd-Parity\t%hu\n", pairs);
        return 0;
    }

    printf("Even-Parity\t%hu\n", pairs);
    return 0;
}
