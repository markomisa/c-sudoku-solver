#include "tree.h"
#include "queue.h" // For freeTree
#include "utils.h" // For freeMatrix
#include <stdio.h> // For NULL

TreeNode *createTreeNode(int **table, int numChildren, int depth)
{
    TreeNode *result = (TreeNode *)malloc(sizeof(TreeNode));
    if (result != NULL)
    {
        result->numChildren = numChildren;
        result->table = table;
        result->depth = depth;
        result->children = (TreeNode **)malloc(numChildren * sizeof(TreeNode *));
    }
    return result;
}

void freeTree(TreeNode *root)
{
    if (root == NULL)
        return;

    Queue *q = createQueue();
    insertQ(q, root);
    while (q->front != NULL)
    {
        TreeNode *current = deleteQ(q);
        for (int i = 0; i < current->numChildren; i++)
        {
            if (current->children[i] != NULL)
            {
                insertQ(q, current->children[i]);
            }
        }
        // Free the resources of the current node
        freeMatrix(current->table, 4); // Assuming n=4
        free(current->children);       // Free the children pointer array
        free(current);                 // Free the node itself
    }
    free(q);
}