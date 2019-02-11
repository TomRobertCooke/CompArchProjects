#include <stdio.h>
#include <stdlib.h>

typedef struct tree
{
    int data;
    struct tree* left;
    struct tree* right;
} tree;

void insert(tree* current, int num)
{
    if (current == NULL)
    {
        current = (tree*) malloc(sizeof(tree));
        current->data = num;
        current->left = NULL;
        current->right = NULL;
        return;
    }
    if (current->data == num)
    {
        return;
    }
    if (num < current->data)
    {
        insert(current->left, num);
        return;
    }
    if (num > current->data)
    {
        insert(current->right, num);
        return;
    }
    return;
}

void inorderPrint(tree* current)
{
    if (current == NULL)
    {
        return;
    }
    if (current->left != NULL)
    {
        inorderPrint(current->left);
    }
    printf("%d\t", current->num);
    if (current->right != NULL)
    {
        inorderPrint(current->right);
    }
    printf("\n");
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

    tree* top = (tree*) malloc(sizeof(tree));
    top = NULL;

    int num = 0;
    if (fscanf(filename, "i %d", &num) <= 0)
    {
        printf("error\n");
        return 0;
    }
    insert(top, num);

    while (fscanf(filename, "\ni %d", &num))
    {
        insert(top, num);
    }

    print(top);
    //printf("\n");

    return 0;
}
