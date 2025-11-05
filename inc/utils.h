#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

void printMatrix(int **matrix, int n);

int **copyMatrix(int **matrix, int n);

void freeMatrix(int **matrix, int n);

int **readMatrix(int n);

int **readMatrixFile(int n, char *name);

#endif // UTILS_H