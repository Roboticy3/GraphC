#ifndef QUEUE_H_
#define QUEUE_H_

#include <stddef.h>

struct queue {
  char* buffer;
  size_t front;
  size_t back;
  size_t capacity;
} typedef queue;

size_t queue_size(queue q);

int queue_push(queue* q, void* content, size_t bytes);

int queue_pop(queue* q, void* output, size_t bytes);

void print_queue(queue q);

queue qalloc(size_t bytes);

void qfree(queue q);

#endif