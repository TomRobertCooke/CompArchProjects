#include <stdlib.h>
#include <stdio.h>

typedef struct node
{
	int data;
	struct node* next;
} node;

int insert(node** table, int num)
{
    if (table[num % 10000] == 0)
    {
        table[num % 10000] = malloc(sizeof(node));
        table[num % 10000]->data = num;
        table[num % 10000]->next = NULL;
    } else
    {
        node* ptr = table[num % 10000];
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
    if (table[num % 10000] != 0)
    {
        node* ptr = table[num % 10000];
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

	char mode;
	int data;
	FILE* filename = fopen(argv[1], "r");
	if (fscanf(filename, "%c %d", &mode, &data) > 0)
	{
		if (mode == 'i')
		{
			if (insert() == 1)
			{
				collisions++;
			}
		} else if (mode == 's')
        	{
            		if (search() == 1)
            		{
                		successes++;
            		}
        	}
	} else
    {
        printf("0\n0\n");
        return 0;
    }

    while (fscanf(filename, "\n%c %d", &mode, &data) > 0)
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
