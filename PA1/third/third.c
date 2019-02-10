#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef struct node
{
	int data;
	struct node* next;
} node;

int abs(int original)
{
	if (original < 0)
	{
		return (original * -1);
	}
	return original;
}

int insert(node** table, int num)
{
    if (table[abs(num) % 10000] == 0)
    {
        table[abs(num) % 10000] = malloc(sizeof(node));
        table[abs(num) % 10000]->data = num;
        table[abs(num) % 10000]->next = NULL;
    } else
    {
        node* ptr = table[abs(num) % 10000];
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = malloc(sizeof(node));
        ptr->next->data = num;
        ptr->next->next = NULL;
        return 1;
    }
    return 0;
}

int search(node** table, int num)
{
    if (table[abs(num) % 10000] != 0)
    {
        node* ptr = table[abs(num) % 10000];
        while (ptr != NULL)
        {
            if (ptr->data == num)
            {
                return 1;
            }
            ptr = ptr->next;
        }
    }
    return 0;
}

int main(int argc, char** argv) /* mod tablesize is the hash function function that we will be using */
{
	if (argc != 2)
	{
		printf("error\n");
		return 0;
	}

	int collisions, successes;
    collisions = 0;
    successes = 0;
	node** table = calloc(10000, sizeof(node*));

	char mode = ' ';
	int data = 0;
	FILE* filename = fopen(argv[1], "r");
	if (filename == NULL)
	{
		printf("error\n");
		return 0;
	}
	if (fscanf(filename, "%c\t%d", &mode, &data) > 0)
	{
		if (mode == 'i')
		{
			if (insert(table, data) == 1)
			{
				collisions++;
			}
		} else if (mode == 's')
        {
            if (search(table, data) == 1)
            {
            	successes++;
            }
        }
	} else
    {
        printf("0\n0\n");
        return 0;
    }

    while (fscanf(filename, "\n%c\t%d", &mode, &data) > 0)
    {
		if (mode == 'i')
		{
			if (insert(table, data) == 1)
			{
				collisions++;
			}
		} else if (mode == 's')
        {
            if (search(table, data) == 1)
            {
                successes++;
            }
        }
    }

    printf("%d\n%d\n", collisions, successes);

    return 0;
}
