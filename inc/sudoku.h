#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>
#include "tree.h"
#include "queue.h"

int **makeSolution(int **matrix, int n, int number);

int checkNumbers(int niz[]);

bool isValid(int **matrica, int n);

int countZero(int **matrix, int n);

void makeTree(TreeNode *root);

void levelOrderTraversal(TreeNode *root);

void writeSolutions(TreeNode *root);

bool isThereSolutions(TreeNode *root);

TreeNode *giveHint(TreeNode *root);

void printSolutionFromList(Queue *q, int n);

void clearSolutionq(Queue *q, int depth);

#endif // SUDOKU_H