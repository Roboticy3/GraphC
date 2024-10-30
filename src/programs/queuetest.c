#include <programs/queuetest.h>
#include <collections/queue.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//thanks ChatGPT!

// Helper function to initialize the queue
void queue_init(queue* q, size_t capacity) {
    q->buffer = (char*)malloc(capacity);
    q->capacity = capacity;
    q->front = 0;
    q->back = 0;
}

// Helper function to destroy the queue
void queue_destroy(queue* q) {
    free(q->buffer);
}

// Test case: Push a single item
void test_push_single_item() {
    queue q;
    queue_init(&q, 10);

    char data = 'A';
    assert(queue_push(&q, &data, 1) == 0);
    assert(queue_size(q) == 1);

    queue_destroy(&q);
    printf("test_push_single_item passed.\n");
}

// Test case: Pop a single item
void test_pop_single_item() {
    queue q;
    queue_init(&q, 10);

    char data = 'A';
    char output;
    queue_push(&q, &data, 1);
    assert(queue_pop(&q, &output, 1) == 0);
    assert(output == 'A');
    assert(queue_size(q) == 0);

    queue_destroy(&q);
    printf("test_pop_single_item passed.\n");
}

// Test case: Push until capacity is reached
void test_push_until_capacity() {
    queue q;
    queue_init(&q, 10);

    char data[9] = "ABCDEFGH";
    assert(queue_push(&q, data, 9) == 0);
    assert(queue_size(q) == 9);
    // Trying to push beyond capacity should return error
    assert(queue_push(&q, "Z", 1) == 1);

    queue_destroy(&q);
    printf("test_push_until_capacity passed.\n");
}

// Test case: Wrap-around push and pop
void test_wrap_around() {
    queue q;
    queue_init(&q, 10);

    char data1[5] = "ABCD";
    char data2[5] = "EFGH";
    char output[5];

    // Fill part of the buffer
    assert(queue_push(&q, data1, 4) == 0);
    assert(queue_pop(&q, output, 4) == 0);

    // Ensure buffer reuses space properly
    assert(queue_push(&q, data2, 4) == 0);
    assert(queue_pop(&q, output, 4) == 0);
    assert(strncmp(output, data2, 4) == 0);

    queue_destroy(&q);
    printf("test_wrap_around passed.\n");
}

// Test case: Pop more bytes than available
void test_pop_more_than_available() {
    queue q;
    queue_init(&q, 10);

    char data[5] = "DATA";
    char output[5];
    assert(queue_push(&q, data, 4) == 0);

    // Attempt to pop more than available should return error
    assert(queue_pop(&q, output, 5) == 1);

    queue_destroy(&q);
    printf("test_pop_more_than_available passed.\n");
}

// Test case: Push with zero bytes
void test_push_zero_bytes() {
    queue q;
    queue_init(&q, 10);

    char data[1] = "X";
    assert(queue_push(&q, data, 0) == 0);
    assert(queue_size(q) == 0);

    queue_destroy(&q);
    printf("test_push_zero_bytes passed.\n");
}

// Test case: Push and pop pattern with alternating data
void test_alternating_push_pop() {
    queue q;
    queue_init(&q, 10);

    char data1 = 'X';
    char data2 = 'Y';
    char output;

    // Push and pop alternately
    assert(queue_push(&q, &data1, 1) == 0);
    assert(queue_pop(&q, &output, 1) == 0);
    assert(output == 'X');

    assert(queue_push(&q, &data2, 1) == 0);
    assert(queue_pop(&q, &output, 1) == 0);
    assert(output == 'Y');

    // Check queue is empty
    assert(queue_size(q) == 0);

    queue_destroy(&q);
    printf("test_alternating_push_pop passed.\n");
}

// Test case: Push and pop with exact capacity fill
void test_exact_capacity_fill() {
    queue q;
    queue_init(&q, 10);

    char data[9] = "12345678";
    char output[9];

    // Fill queue to exact capacity - 1
    assert(queue_push(&q, data, 8) == 0);
    assert(queue_size(q) == 8);

    // Pop everything
    assert(queue_pop(&q, output, 8) == 0);
    assert(strncmp(output, data, 8) == 0);

    // Queue should be empty again
    assert(queue_size(q) == 0);

    queue_destroy(&q);
    printf("test_exact_capacity_fill passed.\n");
}

// Test case: Multiple wrap-around operations
void test_multiple_wrap_around() {
    queue q;
    queue_init(&q, 10);

    char data1[5] = "ABCDE";
    char data2[5] = "FGHIJ";
    char output[5];

    // First wrap-around sequence
    assert(queue_push(&q, data1, 5) == 0);
    assert(queue_pop(&q, output, 5) == 0);
    assert(strncmp(output, data1, 5) == 0);

    // Second wrap-around sequence
    assert(queue_push(&q, data2, 5) == 0);
    assert(queue_pop(&q, output, 5) == 0);
    assert(strncmp(output, data2, 5) == 0);

    queue_destroy(&q);
    printf("test_multiple_wrap_around passed.\n");
}

// Test case: Stress test with large data sets
void test_stress_large_data() {
    queue q;
    queue_init(&q, 1024);

    // Fill with a large dataset
    char* data = (char*)malloc(1023);
    memset(data, 'A', 1023);

    assert(queue_push(&q, data, 1023) == 0);
    assert(queue_size(q) == 1023);

    char* output = (char*)malloc(1023);
    assert(queue_pop(&q, output, 1023) == 0);
    assert(memcmp(output, data, 1023) == 0);

    free(data);
    free(output);

    queue_destroy(&q);
    printf("test_stress_large_data passed.\n");
}

// Test case: Push after full pop to verify queue reusability
void test_reusability_after_emptying() {
    queue q;
    queue_init(&q, 10);

    char data[5] = "HELLO";
    char output[5];

    // Push and then pop all elements
    assert(queue_push(&q, data, 5) == 0);
    assert(queue_pop(&q, output, 5) == 0);
    assert(strncmp(output, data, 5) == 0);

    // Push again after emptying
    char data2[4] = "BYE";
    assert(queue_push(&q, data2, 3) == 0);
    assert(queue_pop(&q, output, 3) == 0);
    assert(strncmp(output, data2, 3) == 0);

    queue_destroy(&q);
    printf("test_reusability_after_emptying passed.\n");
}

// Test case: Edge case with zero capacity queue
void test_zero_capacity_queue() {
    queue q;
    queue_init(&q, 0);

    char data = 'A';
    assert(queue_push(&q, &data, 1) == 1);  // Expect error on push

    queue_destroy(&q);
    printf("test_zero_capacity_queue passed.\n");
}

// Main function to run tests
int queuetest() {
    test_push_single_item();
    test_pop_single_item();
    test_push_until_capacity();
    test_wrap_around();
    test_pop_more_than_available();
    test_push_zero_bytes();
    test_alternating_push_pop();
    test_exact_capacity_fill();
    test_multiple_wrap_around();
    test_stress_large_data();
    test_reusability_after_emptying();
    test_zero_capacity_queue();

    printf("All tests passed.\n");
    return 0;
}
