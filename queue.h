#ifndef QUEUE_H
#define QUEUE_H

#include "btree.h"

typedef struct QueueNode {
    BTreeNode* data;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

Queue* create_queue();
void enqueue(Queue* q, BTreeNode* data);
BTreeNode* dequeue(Queue* q);
int is_empty(Queue* q);
void destroy_queue(Queue* q);

#endif
