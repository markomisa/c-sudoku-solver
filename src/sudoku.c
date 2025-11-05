#include "sudoku.h"
#include "utils.h" // For makeSolution, countZero, printMatrix
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int **makeSolution(int **matrix, int n, int number)
{
    int **result = (int **)malloc(n * sizeof(int *));

    for (int i = 0; i < n; i++)
    {
        result[i] = (int *)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++)
        {
            result[i][j] = matrix[i][j];
        }
    }

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 0)
            {
                result[i][j] = number;
                return result;
            }
        }
    }
    return result;
}

int checkNumbers(int niz[])
{
    int cnt[5]; // Assumes numbers are 0-4
    for (int i = 0; i < 5; i++)
        cnt[i] = 0;
    for (int i = 0; i < 4; i++)
    {
        if (niz[i] != 0)
        { // Only check non-empty cells
            cnt[niz[i]]++;
        }
    }
    for (int i = 1; i <= 4; i++)
        if (cnt[i] > 1)
            return 0; // Duplicate found
    return 1;
}

bool isValid(int **matrica, int n)
{
    int niz[4];
    // Check rows
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            niz[j] = matrica[i][j];
        }
        if (checkNumbers(niz) == 0)
            return false;
    }
    // Check columns
    for (int j = 0; j < n; j++)
    {
        for (int i = 0; i < n; i++)
        {
            niz[i] = matrica[i][j];
        }
        if (checkNumbers(niz) == 0)
            return false;
    }
    // Check 2x2 blocks
    for (int i = 0; i < n; i += 2) // n=4, so i=0, i=2
    {
        for (int j = 0; j < n; j += 2) // j=0, j=2
        {
            niz[0] = matrica[i + 0][j + 0];
            niz[1] = matrica[i + 0][j + 1];
            niz[2] = matrica[i + 1][j + 0];
            niz[3] = matrica[i + 1][j + 1];
            if (checkNumbers(niz) == 0)
                return false;
        }
    }
    return true;
}

int countZero(int **matrix, int n)
{
    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == 0)
            {
                cnt++;
            }
        }
    }
    return cnt;
}

void makeTree(TreeNode *root)
{
    Queue *q = createQueue();
    insertQ(q, root);
    int cnt = countZero(root->table, 4);

    while (q->front != NULL)
    {
        TreeNode *current = deleteQ(q);
        // Only expand nodes that are valid AND not fully solved
        if (isValid(current->table, 4) && cnt != current->depth)
        {
            current->numChildren = 0; // We will count valid children
            current->children = (TreeNode **)calloc(4, sizeof(TreeNode *));
            if (current->children == NULL) exit(1); // Malloc check

            for (int number = 1; number <= 4; number++)
            {
                int **childTable = makeSolution(current->table, 4, number);

                if (isValid(childTable, 4))
                {
                    TreeNode *child = createTreeNode(childTable, 0, current->depth + 1);
                    insertQ(q, child);
                    current->children[current->numChildren++] = child;
                }
                else
                {
                    // If the move is not valid, free the table
                    freeMatrix(childTable, 4);
                }
            }
            // Resize children array to exact number of valid children
            if (current->numChildren > 0) {
                 current->children = (TreeNode **)realloc(current->children, current->numChildren * sizeof(TreeNode *));
            } else {
                free(current->children);
                current->children = NULL;
            }
        }
        else
        {
            // This node is either invalid or a leaf, so no children
            current->numChildren = 0;
            current->children = NULL; // No need to allocate
        }
    }
    free(q);
}

void levelOrderTraversal(TreeNode *root)
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
            insertQ(q, current->children[i]);
        }
        printf("\nNivo stabla: %d\n", current->depth);
        printMatrix(current->table, 4);
    }
    free(q);
}

void printSolutionFromList(Queue *q, int n)
{
    QueueNode *curr = q->front;
    for (int i = 0; i < n; i++)
    {
        if (curr == NULL) break; // Safety check
        printf("\nKorak %d:\n", i);
        printMatrix(curr->treenode->table, 4);
        curr = curr->next;
    }
}

void clearSolutionq(Queue *q, int depth)
{
    if (q->front == NULL) return;

    QueueNode *curr = q->front;
    // Find the node *before* the one to cut
    while (curr->next != NULL && depth > curr->next->treenode->depth)
    {
        curr = curr->next;
    }

    QueueNode *pom = curr->next;
    curr->next = NULL;
    q->rear = curr;

    // Free the rest of the list
    while (pom != NULL)
    {
        curr = pom;
        pom = pom->next;
        // Note: We DO NOT free the tree nodes or tables here, only the queue nodes
        free(curr);
    }
}

void writeSolutions(TreeNode *root)
{
    Queue *solutionQ = createQueue(); // Stores the current path
    Queue *q = createQueue();         // Acts as a stack for DFS
    pushStackFront(q, root);          // Start DFS
    
    int numberSolution = 1;
    int hasSolution = 0;

    while (q->front != NULL)
    {
        TreeNode *current = deleteQ(q); // "Pop" from stack (delete from front)
        
        // Prune solution path if backtracking
        if (solutionQ->front != NULL && current->depth <= solutionQ->rear->treenode->depth) {
            clearSolutionq(solutionQ, current->depth);
        }
        insertQ(solutionQ, current); // Add current node to path

        if (current->numChildren != 0)
        {
            // Push children onto stack (in reverse for natural order)
            for (int i = current->numChildren - 1; i >= 0; i--)
            {
                pushStackFront(q, current->children[i]);
            }
        }
        else // Leaf node
        {
            if (countZero(current->table, 4) == 0) // It's a full, valid solution
            {
                printf("Solution no.: %d\n", numberSolution++);
                printSolutionFromList(solutionQ, current->depth + 1);
                hasSolution = 1;
            }
            // Backtrack: remove this leaf from the solution path
            popQ(solutionQ); 
        }
    }
    if (hasSolution == 0)
    {
        printf("\n-----------------------------------------------------\n");
        printf("\nNo solution\n");
    }
    free(q);
    free(solutionQ);
}

bool isThereSolutions(TreeNode *root)
{
    Queue *solutionQ = createQueue();
    Queue *q = createQueue();
    pushStackFront(q, root);
    
    while (q->front != NULL)
    {
        TreeNode *current = deleteQ(q);
        
        if (solutionQ->front != NULL && current->depth <= solutionQ->rear->treenode->depth) {
            clearSolutionq(solutionQ, current->depth);
        }
        insertQ(solutionQ, current);

        if (current->numChildren != 0)
        {
            for (int i = current->numChildren - 1; i >= 0; i--)
            {
                pushStackFront(q, current->children[i]);
            }
        }
        else
        {
            if (countZero(current->table, 4) == 0)
            {
                free(q);
                free(solutionQ);
                return true; // Found a solution
            }
            popQ(solutionQ);
        }
    }
    free(q);
    free(solutionQ);
    return false; // No solutions found
}

TreeNode *giveHint(TreeNode *root)
{
    Queue *solutionQ = createQueue();
    Queue *q = createQueue();
    pushStackFront(q, root);
    
    while (q->front != NULL)
    {
        TreeNode *current = deleteQ(q);
        
        if (solutionQ->front != NULL && current->depth <= solutionQ->rear->treenode->depth) {
            clearSolutionq(solutionQ, current->depth);
        }
        insertQ(solutionQ, current);

        if (current->numChildren != 0)
        {
            for (int i = current->numChildren - 1; i >= 0; i--)
            {
                pushStackFront(q, current->children[i]);
            }
        }
        else
        {
            if (countZero(current->table, 4) == 0)
            {
                // Found a solution! Return the node after the root.
                TreeNode *hintNode = NULL;
                if (solutionQ->front != NULL && solutionQ->front->next != NULL) {
                    hintNode = solutionQ->front->next->treenode;
                }
                free(q);
                free(solutionQ);
                return hintNode;
            }
            popQ(solutionQ);
        }
    }
    
    printf("This sudoku has no solutions!\n");
    free(q);
    free(solutionQ);
    return NULL;
}