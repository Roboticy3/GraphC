
#include <stddef.h>
#include <collections/array.h>

void reverse(array arr, size_t item_width){
  void* reg = malloc(item_width);
  for (size_t i = 0; i < arr.length / 2; i++) {
    void* back = arr.block + i * item_width;
    void* front = arr.block + item_width * (arr.length - i - 1);
    memcpy(reg, back, item_width);
    memcpy(back, front, item_width);
    memcpy(front, reg, item_width);
  }

  free(reg);
}