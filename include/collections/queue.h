// (c) 2025 Pablo Ibarz
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef QUEUE_H_
#define QUEUE_H_

#include <stddef.h>

// Simple circular buffer, used for breadth-first-search
// `capacity` refers to the allocated block size in bytes, `buffer` points to
// that block. `front` and `back` are byte-indices into `buffer` defining the 
// start and end of the queue.
struct queue {
  char* buffer;
  size_t front;
  size_t back;
  size_t capacity;
} typedef queue;

size_t queue_size(queue q);

int queue_push(queue* q, void* content, size_t bytes);

int queue_pop(queue* q, void* output, size_t bytes);

// Prints the characters in the given queue
void print_queue(queue q);

#endif