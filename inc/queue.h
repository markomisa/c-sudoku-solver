#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h>
#include "tree.h" // Needs TreeNode definition

// Node for the queue, holds a pointer to a TreeNode
typedef struct QueueNode
{
    TreeNode *treenode;
    struct QueueNode *next;
} QueueNode;

// The queue structure
typedef struct Queue
{
    QueueNode *rear;  // Back of the queue
    QueueNode *front; // Front of the queue
} Queue;

Queue *createQueue();

QueueNode *createQueueNode(TreeNode *node);

void insertQ(Queue *q, TreeNode *treeNode);

TreeNode *deleteQ(Queue *q);

TreeNode *popQ(Queue *q);

void pushStackFront(Queue *q, TreeNode *node);

TreeNode *topQ(Queue *q);

#endif // QUEUE_H