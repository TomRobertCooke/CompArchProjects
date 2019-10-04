#include <stdio.h>
#include <stdlib.h>

typedef struct tree
{
    int data;
    struct tree* left;
    struct tree* right;
} tree;

void insert(tree** current, int num)
{
    if (*current == NULL)
	{
		return;
	}
	if ((*current)->data == num)
   	{
       	return;
   	}
   	if (num < (*current)->data)
	{
    	if ((*current)->left != NULL)
		{
			insert(&((*current)->left), num);
			return;
		}
		(*current)->left = (tree*) malloc(sizeof(tree));
		(*current)->left->data = num;
		(*current)->left->left = NULL;
		(*current)->left->right = NULL;
    	return;
	}
	if (num > (*current)->data)
	{
    if ((*current)->right != NULL)
		{
			insert(&((*current)->right), num);
			return;
		}
		(*current)->right = (tree*) malloc(sizeof(tree));
		(*current)->right->data = num;
		(*current)->right->left = NULL;
		(*current)->right->right = NULL;
	return;
    }
    return;
}

void inorderPrint(tree** current)
{
	if (*current == NULL)
    { 
		return;
    }
   	inorderPrint(&((*current)->left));
    printf("%d\t", (*current)->data);
    inorderPrint(&((*current)->right));
    return;
}

void preorderDelete(tree** current)
{
	if (*current == NULL)
	{
		return;
	}
	preorderDelete(&((*current)->left));
	preorderDelete(&((*current)->right));
	if ((*current)->left != NULL)
	{
		free((*current)->left);
	}
	if ((*current)->right != NULL)
	{
		free((*current)->right);
	}
	return;
}

int main(int argc, char** argv)
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

    int num = 0;
    if (fscanf(filename, "i %d", &num) <= 0)
    {
        printf("error\n");
        return 0;
    }
	//printf("%d\n", num);	

	tree** topPtr = (tree**) malloc(sizeof(tree*));
	*topPtr = (tree*) malloc(sizeof(tree));
  
	(*topPtr)->data = num;
	(*topPtr)->left = NULL;
	(*topPtr)-> right = NULL;

    while (fscanf(filename, "\ni %d", &num) > 0)
    {
        //printf("%d\n", num);
		insert(topPtr, num);
    }

    inorderPrint(topPtr);
    printf("\n");
	
	preorderDelete(topPtr);
	free(*topPtr);
	free(topPtr);
	free(filename);

    return 0;
}
