#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

Queue* queue_create(void);
void queue_destroy(Queue* q);
void queue_push(Queue* q, char* str);
void queue_top(Queue* q, char* str);
char* queue_pop(Queue* q);
unsigned int queue_length(const Queue* q);
unsigned int queue_size(const Queue* q);
char* queue_head(const Queue* q);
char* queue_tail(const Queue* q);

#endif
