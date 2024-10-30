#include <stddef.h>
#include <stdio.h>

#include <combinations/choose.h>

/**
 * Mapping from i to k-tuples of [n]. Dumps output into out. Assumes i < (n choose k) and n > k.
 * 
 * Increments through the permutations of n choose k such that the 0'th position is always increasing. O(i)
 * 
 * (5, 3, 0) -> (1, 2, 3)
 * (5, 3, 1) -> (1, 2, 4)
 * (5, 3, 2) -> (1, 2, 5)
 * (5, 3, 3) -> (1, 3, 4)
 * (5, 3, 4) -> (1, 3, 5)
 * (5, 3, 5) -> (1, 4, 5)
 * (5, 3, 6) -> (2, 3, 4)
 * (5, 3, 7) -> (2, 3, 5)
 * (5, 3, 8) -> (2, 4, 5)
 * (5, 3, 9) -> (3, 4, 5)
 */
void choose_i(size_t n, size_t k, size_t i, size_t* out) {

  //fill "out" to then modify
  for (size_t j = 0; j < k; j++) {
    out[j] = j + 1;
  }

  //at each step, increment the last position
  size_t j = k - 1;
  while (i) {
    choose_step(n, k, out);
    i--;
  }
}

void choose_step(size_t n, size_t k, size_t* out) {
  //reset j to the last position
  size_t j = k - 1;
  
  out[j]++;

  //the "max value" for each position is n - j, adjusted for 0-indexing
  //if a position exceeds its max value, scan for the last value in out where incrementing doesn't result in exceeding the max value
  while (out[j] > n - (k - j) + 1 && j > 0) {
    j--;
    out[j]++;
  }

  //then set all positions after that one to its value +1, +2, etc.
  //this is sort of like setting all positions to 0 when counting from 999 to 1000, except in a way that avoids duplicate digits and hence retains that out is a valid permutation
  j++;
  while (j < k) {
    out[j] = out[j-1] + 1;
    j++;
  }
  
}

size_t choose(size_t n, size_t k) {
  double x = 1.0;
  for (double i = 1.0; i <= k; i += 1.0) {
    double factor = (n - k + i) / i;
    x *= factor;
    //printf("%f, %f\n", x, factor );
  }
  return x + 0.01;
}