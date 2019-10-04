#include <stdlib.h>
#include <stdio.h>

void print(int rows, int cols, int* matrix)
{
	int i, j;
	for (i = 0; i < rows; i++)
    {
        for (j = 0; j < cols; j++)
        {
            printf("%d\t", *(matrix + cols * i + j));
        }
        printf("\n");
    }	
	return;
}

int product(int first, int second)
{
	//printf("first times second = %d\n", first * second);
	return first * second;
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

    int rows1, cols1, rows2, cols2, i, j, k, *matrix1, *matrix2;
    if (fscanf(filename, "%d %d\n", &rows1, &cols1) <= 0)
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
                fscanf(filename, "%d", matrix1 + i * cols1 + j);
            }
        }
    }

    if (fscanf(filename, "%d %d\n", &rows2, &cols2) <= 0)
    {
        printf("error\n");
        return 0;
    } else
    {
        matrix2 = (int*) malloc(rows2 * cols2 * sizeof(int));
        for (i = 0; i < rows2; i++)
        {
            for (j = 0; j < cols2; j++)
            {
                fscanf(filename, "%d", matrix2 + i * cols2 + j);
            }
        }
    }

	if (cols1 != rows2)
	{
        free(filename);
        free(matrix1);
        free(matrix2);
		printf("bad-matrices\n");
		return 0;
	}

    int* result = malloc(rows1 * cols2 * sizeof(int));
    int sum;

    for (k = 0; k < rows1; k++)
    {
        for (i = 0; i < cols2; i++)
        {
            sum = 0;
            for (j = 0; j < cols1; j++)
            {
                sum = sum + product(*(matrix1 + k * cols1 + j), *(matrix2 + j * cols2 + i));
				//printf("%d = sum, %d times %d\n", sum, *(matrix1 + k * cols1 + j), *(matrix2 + j * cols2 + i));
            }
            *(result + i + k * cols2) = sum;
        }
    }

	free(matrix1);
	free(matrix2);

	//print(rows1, cols1, matrix1);
	//printf("\n");
	//print(rows2, cols2, matrix2);
	//printf("\n");
	print(rows1, cols2, result);
    free(result);
    free(filename);
    return 0;
}
