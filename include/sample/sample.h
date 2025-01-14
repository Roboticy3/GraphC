// (c) 2025 Pablo Ibarz
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <stddef.h>
#include <combinations/xoshiro.h>
#include <threads.h>
#include <sqlite3.h>
#include <graph/graph.h>

// `len` samples of discrete data points and their continuous mean.
struct sample {
  size_t* x;
  size_t len;
  double mean;
} typedef sample;

// Data about a sample.
// in this case, that means data about the type of graph is was sampling, 
// including order, edge probability, and the number of random graphs samples/
// This structure ended up being used as parameters to generate random graph
// samples, so it also carries a thread-safe random state.
struct sampledata {
  size_t order;
  float p;
  size_t chunk_size;
  pXSR random_state;
} typedef sampledata;

// A range over which to randomly sample graphs, defined by a range of order
// and edge probability
// `order_step` and `p_step` define the resolution of the range. Smaller steps
// = higher computation time and vice-versa.
struct binomialrange {
  size_t order_min;
  size_t order_max;
  size_t order_step;
  float p_min;
  float p_max;
  float p_step;
} typedef binomialrange;

// A wrapper for binomialrange with extra data about how many samples to take,
// and a random seed.
// Each range is intended to be processed on a single thread, so only one seed
// is needed for each.
struct binomialrangedata {
  uint64_t seed;
  binomialrange range;
  size_t sample_count;
  size_t range_count;
} typedef binomialrangedata;

//Macros for iterating through a binaryrange

// binaryrange loop macro start
// Used in tandem with END_BRL to iterate through each point in a binaryrange
#define START_BRL(range, i, j, o, p) \
  o = range.order_min; for (size_t i = 0; o < range.order_max; i++) { \
  p = range.p_min; for (size_t j = 0; p < range.p_max; j++) {

// binaryrange loop macro end
// Used in tangem with START_BRL to iterate through each point in a binaryrange
#define END_BRL(range, i, j, o, p) \
  p += range.p_step;}\
  o += range.order_step;}

// Measure some discrete property `*property` into a buffer of sample means 
// over random graphs in the range `s`
void sample_range(binomialrangedata s, double* out, size_t (*property)(NeighborhoodGraph g));

// Measure one sample of discrete property `*property` into `out` with sample
// parameters `params`
void sample_point(sampledata params, sample* out, size_t (*property)(NeighborhoodGraph g));

// Macros for indexing a one-dimensional array representing a matrix of samples

// Given a binaryrange, R_STRIDE computes the length of a row of the matrix,
// while R_HEIGHT computes the number of rows.
#define R_STRIDE(r) ((r.p_max - r.p_min) / r.p_step)
#define R_HEIGHT(r) ((r.order_max - r.order_min) / r.order_step)

// Get the order of a certain row `i` in `r`
size_t get_row(binomialrange r, size_t i);

// Get the edge probability of a certain column `i` in `r`
float get_col(binomialrange r, size_t i);

#endif 