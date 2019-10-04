#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	int num;
	struct Node* next;
} Node;

int count = 0;

void printList(Node* front)
{

	/*printf("%d looking for free use\n", front->num);*/
	Node* ptr = front;
	
	if (front == NULL)
	{
		printf("0\n\n");
		return;
	}
	
	printf("%d\n", count);
	int prev;
	while (ptr != NULL)
	{
		if (ptr->num != prev) 
		{
			printf("%d\t", ptr->num);
			prev = ptr->num;
		}
		ptr = ptr->next;
	}
	printf("\n");
	return;
}

void insert(Node** front, int num)
{
	Node* new = (Node*) malloc(sizeof(Node));
	new->num = num;
	Node* ptr = *front;

	if (num < (*front)->num)
	{
		*new = **front;
		(*front)->num = num;
		(*front)->next = new;
		return;
	}

	while (ptr->next != NULL)
	{	
		if (num < ptr->next->num) 
		{
			new->next = ptr->next;
			ptr->next = new;
			return;
		}
		ptr = ptr->next;
	}

	ptr->next = new;
	return;
} //mem leak when 5 is inserted occurs here

int delete(Node** front, int searchNum)
{
	Node* ptr = *front;
	if (*front == NULL) 
	{
		return 0;
	} else if ((*front)->num == searchNum)
	{
		if ((*front)->next == NULL)
		{
			return 2;
		}
		return 3;
	}
	
	Node* prev;
	while (ptr != NULL)
	{
		if (ptr->num == searchNum)
		{
			prev->next = ptr->next;
			free(ptr);
			return 1;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return 0;
}

void deleteList(Node* current)
{
	if (current == NULL)
	{
		return;
	}
	deleteList(current->next);
	free(current);
	return;
}

int main(int argc, char* argv[])
{
	
	if (argc != 2) 
	{
		printf("error\n");
		return 0;
	}

	FILE* filename = fopen(argv[1], "r");
	if (filename == NULL) 
	{
		printf("error\n");
		return 0;
	}

	char mode;
	int num;
	if (fscanf(filename, "%c %d\n", &mode, &num) <= 0) 
	{
		printf("0\n\n");
		return 0;
	}

	Node* front = NULL;

	if (mode == 'i')
	{
		front = (Node*) malloc(sizeof(Node));
		front->num = num;
		front->next = NULL;
		count = 1;
	}

	while(fscanf(filename, "%c %d\n", &mode, &num) > 0)
	{
		/*printf("%c %d mode and num\n", mode, num);*/
		if (mode == 'i')
		{
			if (front == NULL)
			{
				
				front = (Node*) malloc(sizeof(Node));
				front->num = num;
				front->next = NULL;
				count++;
			} else
			{
				Node** frontPtr = (Node**) malloc(sizeof(Node*));
				*frontPtr = front;
				insert(frontPtr, num);
				count++;
				free(frontPtr);
			}
		} else if (mode == 'd')
		{
			Node** frontPtr = (Node**) malloc(sizeof(Node*));
			*frontPtr = front;
			int g = delete(frontPtr, num);
			if (g > 0)
			{
				count--;
			}
			if (g == 2)
			{
				free(front);
				front = NULL;
			} if (g == 3)
			{
				Node* ptr = front->next;
				free(front);
				front = ptr;
			}
			/*printList(front);
		    printf("-----------------------\n");*/
			free(frontPtr);
		}
		/*printList(front);
		printf("-------------------------\n");*/
	}


	if (fscanf(filename, "%c %d", &mode, &num) > 0)
	{
		if (mode == 'i') 
		{
			Node** frontPtr = (Node**) malloc(sizeof(Node*));
			*frontPtr = front;
			insert(frontPtr, num);
			count++;
			free(frontPtr);
		} else if (mode == 'd') 
		{
			Node** frontPtr = (Node**) malloc(sizeof(Node*));
			*frontPtr = front;
			int g = delete(frontPtr, num);
			if (g > 0)
			{
				count--;
			}
			if (g == 2)
			{
				free(front);
				front = NULL;
			} if (g == 3)
			{
				Node* ptr = front->next;
				free(front);
				front = ptr;
			}
			/*printList(front);
		    printf("-----------------------\n");*/
			free(frontPtr);
		}
	}
	
	printList(front);
	deleteList(front);
	
	free(filename);

	return 0;
}
