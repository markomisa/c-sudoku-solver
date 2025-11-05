#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void printMatrix(int **matrix, int n)
{
    if (matrix == NULL)
    {
        printf("NULL");
        return;
    }
    putchar('\n');
    printf("     1:  2:  3:  4:\n");
    printf("    --- --- --- ---\n");
    for (int i = 0; i < n; i++)
    {
        printf("%d: |", i + 1);
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 0)
            {
                printf("   ");
            }
            else
            {
                printf(" %d ", matrix[i][j]);
            }
            putchar('|');
        }
        printf("\n    --- --- --- ---\n"); // Corrected placement
    }
    putchar('\n');
}

int **copyMatrix(int **matrix, int n)
{
    int **result = (int **)malloc(n * sizeof(int *));
    if (result == NULL) return NULL;

    for (int i = 0; i < n; i++)
    {
        result[i] = (int *)malloc(n * sizeof(int));
        if (result[i] == NULL) return NULL; // Check each malloc

        for (int j = 0; j < n; j++)
        {
            result[i][j] = matrix[i][j];
        }
    }
    return result;
}

void freeMatrix(int **matrix, int n)
{
    if (matrix == NULL)
        return;
    for (int i = 0; i < n; i++)
    {
        free(matrix[i]); // Free each row
    }
    free(matrix); // Free the row pointers
}

int **readMatrix(int n)
{
    int i, j;
    int **matrix = (int **)malloc(n * sizeof(int *));
    if (matrix == NULL) exit(1);

    for (i = 0; i < n; i++)
    {
        matrix[i] = (int *)malloc(n * sizeof(int));
        if (matrix[i] == NULL) exit(1);
    }

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &matrix[i][j]);
        }
    }
    return matrix;
}

int **readMatrixFile(int n, char *name)
{
    FILE *ulaz;
    int i, j, **matrix;
    int n_from_file; // Variable to read n from file

    ulaz = fopen(name, "r");

    if (ulaz == NULL)
    {
        perror("File opening failed");
        exit(2);
    }
    
    fscanf(ulaz, "%d", &n_from_file);
    // Note: The passed 'n' parameter is ignored, and the 'n' from the file is used.
    // This matches the original code's behavior.
    
    matrix = (int **)malloc(n_from_file * sizeof(int *));
    if (matrix == NULL) exit(1);

    for (i = 0; i < n_from_file; i++)
    {
        matrix[i] = (int *)malloc(n_from_file * sizeof(int));
        if (matrix[i] == NULL) exit(1);
    }

    for (i = 0; i < n_from_file; i++)
    {
        for (j = 0; j < n_from_file; j++)
        {
            fscanf(ulaz, "%d", &matrix[i][j]);
        }
    }
    fclose(ulaz);
    return matrix;
}