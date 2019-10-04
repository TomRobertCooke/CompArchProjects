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
		return (original + 10000);
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
        //printf("does this get called in the others\n");
		node* ptr = table[abs(num) % 10000];
        while (ptr->next != NULL)
        {
            if (ptr->data == num)
			{
				return 1;
			}
			ptr = ptr->next;
        }
		if (ptr->data == num)
		{
			return 1;
		}
        ptr->next = malloc(sizeof(node));
        ptr->next->data = num;
        ptr->next->next = NULL;
        return 2;
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
	node** table = (node**) calloc(10000, sizeof(node*));
	
	int* nodes = malloc(sizeof(int));
	int curnum = 0;	

	char mode = ' ';
	int data = 0;
	FILE* filename = fopen(argv[1], "r");
	if (filename == NULL)
	{
		printf("error\n");
		free(nodes);
		free(table);
		free(filename);
		return 0;
	}
	if (fscanf(filename, "%c\t%d", &mode, &data) > 0)
	{
		if (mode == 'i')
		{
			int inserted = insert(table, data);
			if (inserted == 1)
			{
				collisions++;
			} else if (inserted == 0 || inserted == 2)
			{
				if (inserted == 2)
				{
					collisions++;
				}
				nodes = realloc(nodes, sizeof(int)*(curnum+1));
				nodes[curnum] = data;
				curnum++;
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
			int inserted = insert(table, data);
			if (inserted == 1)
			{
				collisions++;
				continue;
			} else if (inserted == 0 || inserted == 2)
			{
				if (inserted == 2)
				{
					collisions++;
				}
				nodes = realloc(nodes, sizeof(int)*(curnum+1));
				nodes[curnum] = data;
				curnum++;
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

	
	int i = 0;
	for (i = 0; i < curnum; i++)
	{
		if (table[abs(nodes[i]) % 10000] != 0)
    	{
			node* ptr = table[abs(nodes[i]) % 10000];
			node* prev = NULL;
			if (ptr == NULL)
			{
				continue;
			}
    	    while (ptr != NULL)
    	    {
    	        if (ptr->data == nodes[i])
    	        {
    	            if (prev == NULL)
					{
						ptr = table[abs(nodes[i]) % 10000]->next;
						free(table[abs(nodes[i]) % 10000]);
						table[abs(nodes[i]) % 10000] = ptr;;
						break;
					} else if (ptr->next == NULL)
					{				
						free(ptr);
						prev->next = NULL;
						break;
					} else
					{
						prev->next = ptr->next;
						free(ptr);
						break;
					}
        	    }
				prev = ptr;
        	    ptr = ptr->next;
        	}
		}
	}
	free(nodes);
	free(table);
	free(filename);

    return 0;
}
