#include <stdio.h>
#include <stdlib.h>

void swap(int* first, int* second)
{
	int* temp = (int*) malloc(sizeof(int));
	*temp = *first;
	*first = *second;
	*second = *temp;
	free(temp);
	return;
}

int main(int argc, char* argv[])
{
	int ints[20], length;

	if (argc != 2) 
	{
		printf("argc is %d\n", argc);
		return 0;
	}
	
	
	/* Here we read from input and store the ints in the array */
	
	FILE* input;
	input = fopen(argv[1], "r");
	if (input == NULL) 
	{
		printf("error\n");
		return 0;
	}
	
	int i;
	if (fscanf(input, "%d", &length) <= 0) 
	{
		printf("error\n");
		return 0;
	} else {
		for (i = 0; i < length; i++)
		{
			if (fscanf(input, "%d", &ints[i]) <= 0)
			{
				printf("error\n");
				return 0;
			}
		}
	}
	
	/* Now we perform a bubble sort */
	int j;
	if (length > 1)
	{
		for (i = 0; i < length - 1; i++)
		{
			for (j = 0; j < length - 1; j++)
			{
				if (ints[j] % 2 == 0 && ints[j+1] % 2 == 0) /* Both Even */
				{	
					if (ints[j] > ints[j+1])
					{
						swap(&ints[j], &ints[j+1]);
					}
				} else if (ints[j] % 2 != 0 && ints[j+1] % 2 != 0) /* Both Odd */
				{
					if (ints[j] < ints[j+1])
					{
						swap(&ints[j], &ints[j+1]);
					}
				} else if ((ints[j] % 2 != 0) && (ints[j+1] % 2 == 0))
				{
					swap(&ints[j], &ints[j+1]);
				}
			}
		}
	}
	
	for (i = 0; i < length; i++) { /* Printing the list in order */
		printf("%d\t", ints[i]);
	}
	printf("\n");	
	
	free(input);

	return 0;
}
