#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
	int num;
	struct Node* next;
} Node;

int count = 0;

void insert(Node* first, int num)
{
	Node* new = (Node*) malloc(sizeof(Node));
	new->num = num;
	Node* ptr = first;

	if (num <= ptr->num)
	{
		new->next = (Node*) malloc(sizeof(Node));
		*(new->next) = *ptr;
		*ptr = *new;
		return;
	}

	while (ptr->next != NULL)
	{	
		if (num <= ptr->next->num) 
		{
			new->next = ptr->next;
			ptr->next = new;
			return;
		}
		ptr = ptr->next;
	}

	ptr->next = new;
	return;
}

int delete(Node* first, int searchNum)
{
	Node* ptr = first;
	Node* prev;
	if (ptr == NULL) 
	{
		return 0;
	} else if (ptr->num == searchNum)
	{
		*first = *(ptr->next);
		return 1;
	}
	while (ptr != NULL)
	{
		if (ptr->num == searchNum)
		{
			*(prev->next) = *(ptr->next);
			return 1;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return 0;
}

void printList(Node* first)
{
	int prev;
	Node* ptr = first;
	printf("%d\n", count);
	if (first == NULL)
	{
		printf("0\n\n");
		return;
	}
	while (ptr != NULL)
	{
		if (ptr->num != prev) 
		{
			printf("%d ", ptr->num);
			prev = ptr->num;
		}
		ptr = ptr->next;
	}
	printf("\n");
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
				front= (Node*) malloc(sizeof(Node));
				front->num = num;
				front->next = NULL;
				count++;
				continue;
			} else
			{
				insert(front, num);
				count++;
			}
		} else if (mode == 'd')
		{
			if (delete(front, num)) count--;
		}
	}

	if (fscanf(filename, "%c %d", &mode, &num) > 0)
	{
		if (mode == 'i') 
		{
			insert(front, num);
			count++;
		} else if (mode == 'd') 
		{
			if (delete(front, num)) count--;
		}
	}

	printList(front);

	return 0;
}
