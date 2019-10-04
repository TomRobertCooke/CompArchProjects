#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        printf("error\n");
        return 0;
    }

    int i, j, curSize;
    char cur;
    char* returnString = (char*) malloc(sizeof(char) * 2);
    curSize = 2;

    for (i = 1; i < argc; i++)
    {
        for (j = 0; j < strlen(argv[i]); j++)
        {
            cur = *(argv[i] + j);
            switch (cur)
            {
                case 65 :
                case 69 :
                case 73 :
                case 79 :
                case 85 :
                case 97 :
                case 101 :
                case 105 :
                case 111 :
                case 117 :
                    returnString = realloc(returnString, sizeof(char) * (curSize + 1));
                    sprintf(returnString, "%s%c", returnString, cur);
		    curSize++;
                    break;
            }
        }
    }

    sprintf(returnString, "%s%c", returnString, '\0');
    printf("%s\n", returnString);

    free(returnString);

    return 0;
}
