#include "queue.h"
#include <stdio.h> // For NULL

Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}

QueueNode *createQueueNode(TreeNode *node)
{
    QueueNode *newNode = (struct QueueNode *)malloc(sizeof(struct QueueNode));
    newNode->treenode = node;
    newNode->next = NULL;
    return newNode;
}

void insertQ(Queue *q, TreeNode *treeNode)
{
    QueueNode *node = createQueueNode(treeNode);
    if (q->rear == NULL)
    {
        q->front = node;
    }
    else
    {
        q->rear->next = node;
    }
    q->rear = node;
}

TreeNode *deleteQ(Queue *q)
{
    if (q->rear == NULL) 
    {
        return NULL;
    }
    QueueNode *prvi = q->front;
    TreeNode *result = prvi->treenode;
    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }
    free(prvi);
    return result;
}

TreeNode *popQ(Queue *q)
{
    if (q->rear == NULL)
    {
        return NULL;
    }
    QueueNode *current = q->front;
    TreeNode *result = q->rear->treenode;
    if (q->front == q->rear)
    {
        // Only one element
        q->front = NULL;
        q->rear = NULL;
        free(current);
    }
    else
    {
        // Find the node before the rear
        while (current->next != q->rear)
        {
            current = current->next;
        }
        current->next = NULL;
        free(q->rear);
        q->rear = current;
    }
    return result;
}

void pushStackFront(Queue *q, TreeNode *node)
{
    QueueNode *result = createQueueNode(node);
    if (q->rear == NULL)
    {
        q->front = result;
        q->rear = result;
        return;
    }
    result->next = q->front;
    q->front = result;
}

TreeNode *topQ(Queue *q)
{
    if (q->rear == NULL)
    {
        return NULL;
    }
    return q->rear->treenode;
}