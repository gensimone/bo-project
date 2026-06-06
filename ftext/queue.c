/* Queue - Linked List. Implemented with some unusual methods and fields. */
#include "queue.h"
#include "alloc.h"
#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Node implementation. */
typedef struct Node {
  struct Node* next;
  char* str;
} Node;

/* Queue implementation. */
struct Queue {
  unsigned int size;   // Tracks the number of elements (useful for calculating spaces).
  unsigned int length; // Track the total length of words.
                       // The length of a character is always considered equal to 1, including
                       // non-ASCII UTF-8 characters.
  Node* tail;
  Node* head;
};

/* Create and initialize a Node. */
static Node* node_create(char* str)
{
  Node* n = (Node*) die_on_fail_malloc(sizeof(Node));
  n->next = NULL;
  n->str = str;
  return n;
}

/* Create and initialize a Queue. */
Queue* queue_create(void)
{
  Queue* q = (Queue*) die_on_fail_malloc(sizeof(Queue));
  q->size = 0;
  q->length = 0;
  q->tail = NULL;
  q->head = NULL;
  return q;
}

/* Destroy the queue. */
void queue_destroy(Queue* q)
{
  while (q->head != NULL) {
    Node* n = q->head->next;
    free(q->head->str);
    free(q->head);
    q->head = n;
  }
  q->tail = NULL;
  q->size = 0;
  q->length = 0;
}

/* Push from the bottom. */
void queue_push(Queue* q, char* str)
{
  Node* n = node_create(str);
  if (queue_size(q) == 0)
    q->tail = q->head = n;
  else {
    q->tail->next = n;
    q->tail = n;
  }
  q->size++;
  q->length += rstrlen(str);
}

/* Push from the top. */
void queue_top(Queue* q, char* str)
{
  Node* n = node_create(str);
  if (queue_size(q) == 0)
    q->tail = q->head = n;
  else {
    n->next = q->head;
    q->head = n;
  }

  q->size++;
  q->length += rstrlen(str);
}

/* Remove and return the tail if present, Otherwise NULL. */
char* queue_pop(Queue* q)
{
  if (queue_size(q) == 0)
    return NULL;

  char* out = q->head->str;
  Node* new_head = q->head->next;

  free(q->head);

  q->head = new_head;
  q->size--;
  q->length -= rstrlen(out);

  return out;
}

unsigned int queue_length(const Queue* q)
{
  return q->length;
}

unsigned int queue_size(const Queue* q)
{
  return q->size;
}

char* queue_head(const Queue* q)
{
  return q->head ? q->head->str : NULL;
}

char* queue_tail(const Queue* q)
{
  return q->tail ? q->tail->str : NULL;
}
