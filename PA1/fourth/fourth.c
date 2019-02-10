#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("error\n");
        return 0;
    }

    FILE* filepath = fopen(filename, "r");
    if (filepath == NULL)
    {
        printf("error\n");
        return 0;
    }

    int rows1, cols1, rows2, cols2, i, j, k, **matrix1, **matrix2;
    if (fscanf(filepath, "%d %d\n", &rows1, &cols1) <= 0)
    {
        printf("error\n");
        return 0;
    } else
    {
        matrix1 = (int*) malloc(rows1 * rows2 * sizeof(int));
        for (i = 0; i < rows1; i++)
        {
            for (j = 0; j < cols1; j++)
            {
                fscanf(filepath, "%d", *(matrix1 + i * cols1 + j);
            }
        }
    }

    if (fscanf(filepath, "%d %d\n", &rows2, &cols2) <= 0)
    {
        printf("error\n");
        return 0;
    } else
    {
        matrix2 = (int*) malloc(rows2 * cols2 * sizeof(int));
        for (i = 0; i < rows1; i++)
        {
            for (j = 0; j < cols1; j++)
            {
                fscanf(filepath, "%d", *(matrix2 + i * cols2 + j);
            }
        }
    }

    int** result = malloc(rows1 * cols2 * sizeof(int))
    int sum;

    for (k = 0; k < rows1; k++)
    {
        for (i = 0; i < cols2; i++)
        {
            sum = 0;
            for (j = 0; j < cols1; j++)
            {
                sum += *(matrix1 + k * i + j) * *(matrix2 + k + i * j)
            }
            *(matrix3 + i + k * cols2) = sum;
        }
    }

    for (i = 0; i < rows1; i++)
    {
        for (j = 0; j < cols2; j++)
        {
            printf("%d\t", *(matrix3 + cols2 * i + j));
        }
        printf("\n");
    }

    return 0;
}
