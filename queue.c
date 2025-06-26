#include <stdlib.h>
#include "queue.h"

Queue* create_queue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, BTreeNode* data) {
    QueueNode* node = (QueueNode*)malloc(sizeof(QueueNode));
    node->data = data;
    node->next = NULL;
    if (q->rear) q->rear->next = node;
    else q->front = node;
    q->rear = node;
}

BTreeNode* dequeue(Queue* q) {
    if (!q->front) return NULL;
    QueueNode* temp = q->front;
    BTreeNode* data = temp->data;
    q->front = temp->next;
    if (!q->front) q->rear = NULL;
    free(temp);
    return data;
}

int is_empty(Queue* q) {
    return q->front == NULL;
}

void destroy_queue(Queue* q) {
    while (!is_empty(q)) dequeue(q);
    free(q);
}
