#ifndef TREE_H
#define TREE_H

#include <stdlib.h>

// Define the TreeNode structure
typedef struct TreeNode
{
    int **table;
    int numChildren;
    int depth;
    struct TreeNode **children;
} TreeNode;

TreeNode *createTreeNode(int **table, int numChildren, int depth);

void freeTree(TreeNode *root);

#endif // TREE_H