// (c) 2025 Pablo Ibarz
// This code is licensed under MIT license (see LICENSE.txt for details)

// though... this one is really all knuth's algorithms.

#ifndef CHOOSE_H_
#define CHOOSE_H_

#include <stddef.h>

// Used for enumerating the `k`-tuples on `n` into `out`
// Assumes that `out` is allocated, `k` <= `n`, and `i` <= n choose k, then
// fills `out` with the `i`'th `k`-tuple on `n`:
// 
// Tuples are in lexicographic order such that, if they were the digits of a
// base-k number, the i'th tuple would always be larger than the (i-1)'th
//
// (5, 3, 0, out) -> {1, 2, 3} in out
// (5, 3, 1, out) -> {1, 2, 4} in out
// (5, 3, 2, out) -> {1, 2, 5} in out
// (5, 3, 3, out) -> {1, 3, 4} in out
// (5, 3, 4, out) -> {1, 3, 5} in out
// (5, 3, 5, out) -> {1, 4, 5} in out
// (5, 3, 6, out) -> {2, 3, 4} in out
// (5, 3, 7, out) -> {2, 3, 5} in out
// (5, 3, 8, out) -> {2, 4, 5} in out
// (5, 3, 9, out) -> {3, 4, 5} in out
//
//
void choose_i(size_t n, size_t k, size_t i, size_t* out);

// Used for one step of enumerating the `k`-tuples on `n` into `out`
// assumes `out` is already populated with a `k`-tuple on `n` from the
// lexicographic ordering, and replaces it's contents with the next one.
void choose_step(size_t n, size_t k, size_t* out);

// compute n choose k
size_t choose(size_t n, size_t k);

#endif