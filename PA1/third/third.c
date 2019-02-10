#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
	int data;
	struct node* next;
} node;

int insert(node** table, int num)
{

}

int search(node** table, int num)
{

}

int main(int argc, char** argv) /* mod tablesize is the hash function function that we will be using */
{
	if (argc != 2)
	{
		printf("error\n");
		return 0;
	}

	int collisions = 0;
	node** table = calloc(10000, sizeof(node*));

	char mode;
	int data;
	FILE* filename = fopen(argv[1], "r");
	if (fscanf(filename, "%c %d", &mode, &data) <= 0)
	{
		if (mode == 'i')
		{
			if (insert() == 1)
			{
				collisions++;
			}
		} else ()

	}

	return 0;
}
