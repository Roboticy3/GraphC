
#include <collections/queue.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

size_t queue_size(queue q) {
  if (q.back >= q.front) {
    return q.back - q.front;
  } 

  return q.capacity + q.back - q.front;
}

int queue_push(queue* q, void* content, size_t bytes) {
  //Do nothing when no bytes are pushed
  if (!bytes) {
    return 0;
  }
  
  //Error Case: not enough space!
  if (bytes >= q->capacity - queue_size(*q)) {
    return 1;
  }

  //Case 1: empty space in the middle
  if (q->front > q->back) {
    strncpy(q->buffer + q->back, content, bytes);
    q->back += bytes;
    return 0;
  }

  //Case 2: empty space at the end or empty space at the beginning
  char* content_back = (char*)content;
  char* content_front = 0;

  size_t back_width = q->capacity - q->back;
  size_t front_width = bytes - back_width;
  if (bytes > back_width) {
    content_front = ((char*)content) + back_width;
  } else {
    front_width = 0;
    back_width = bytes;
  }

  strncpy(q->buffer + q->back, content_back, back_width);
  if (front_width) {
    strncpy(q->buffer, content_front, front_width);
    q->back = front_width;
  } else q->back += back_width;

  return 0;
}

int queue_pop(queue* q, void* output, size_t bytes) {
  
  //Error Case: queue does not have enough bytes
  if (bytes > queue_size(*q)) {
    return 1;
  }

  //Case 1: occupied space in the middle
  if (q->back > q->front) {
    strncpy(output, q->buffer + q->front, bytes);
    q->front += bytes;
    return 0;
  }

  //Case 2: occupied space at the end and/or empty space at the beginning
  char* content_back = 0;
  char* content_front = q->buffer + q->front;

  size_t front_width = q->capacity - q->front;
  size_t back_width = bytes - front_width;
  if (bytes > front_width) {
    content_back = q->buffer;
  } else {
    back_width = 0;
    front_width = bytes;
  }

  strncpy(output, content_front, front_width);
  if (back_width) {
    strncpy(output + front_width, content_back, back_width);
    q->front = back_width;
  } else q->back += front_width;

  return 0;


}

void print_queue(queue q) {

  printf("[");
  for (int i = 0; i < q.capacity; i++) {

    if (q.front >= q.back) {
      if (i < q.front && i >= q.back) {
        printf("?");
      } else printf("%x", q.buffer[i]);
    }
    else {
      if (i < q.front || i >= q.back) {
        printf("?");
      } else printf("%x", q.buffer[i]);
    }
  }

  printf("]\n");
}

queue qalloc(size_t bytes) {
  queue q;
  q.buffer = malloc(bytes);
  q.capacity = bytes;
  q.front = 0;
  q.back = 0;
  return q;
}

void qfree(queue q) {
  free(q.buffer);
}