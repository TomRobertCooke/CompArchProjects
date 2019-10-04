#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned short get(unsigned short x, unsigned short n)
{
	unsigned short result = x;
	result = result >> n; //shifting to the left because it is in little endian, the ones place should be all the way to the left
	return result & 1; // bitwise and with one should just return the value of the first bit;
}

unsigned short comp(unsigned short x, unsigned short n)
{
	unsigned short first = x, second = x, nbit;
	first = first >> n;
	nbit = (~first) & 1;
	first = first >> 1;
	first = first << 1;
	first = (first | nbit);
	first = first << n;
	second = second << (16-n);
	second = second >> (16-n);
	return first | second;
}

unsigned short set(unsigned short x, unsigned short n, unsigned short v)
{
	unsigned short first = x, second = x;
	first = first >> (n + 1);
	first = first << 1;
	first = (first | v);
	first = first << n;
	second = second << (16- n);
	second = second >> (16 - n);
	return first | second;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf ("error\n");
		return 0;
	}
	
	FILE* filename = fopen(argv[1], "r");
	if (filename == NULL)
	{
		printf("error\n");
		return 0;
	}
	
	unsigned short x;
	if (fscanf(filename, "%hu", &x) <= 0)
	{
		free(filename);
		printf("error\n");
		return 0;
	}
	
	char* instruction = (char*) malloc(5 * sizeof(char));
	unsigned short n, v;
	while (fscanf(filename, "\n%s %hu %hu", instruction, &n, &v) > 0)
	{
		if (strcmp(instruction, "set") == 0)
		{
			x = set(x, n, v);
			printf("%hu\n", x);
		} else if (strcmp(instruction, "comp") == 0)
		{
			x = comp(x, n);
			printf("%hu\n", x);
		} else if (strcmp(instruction, "get") == 0)
		{
			printf("%hu\n", get(x, n));
		} else
		{
			free(filename);
			printf("badly formated file\n");
			return 0;
		}
	}
	
	free(instruction);
	free(filename);
	return 0;
}
