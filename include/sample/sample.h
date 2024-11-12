#ifndef SAMPLE_H_
#define SAMPLE_H_

#include <stddef.h>
#include <combinations/xoshiro.h>

struct sample {
  size_t* x;
  size_t len;
  double mean;
} typedef sample;

struct sampledata {
  size_t order;
  float p;
  size_t chunk_size;
  pXSR random_state;
} typedef sampledata;

struct binomialrange {
  size_t order_min;
  size_t order_max;
  size_t order_step;
  float p_min;
  float p_max;
  float p_step;
} typedef binomialrange;

struct binomialrangedata {
  uint64_t seed;
  binomialrange range;
  size_t sample_count;
  size_t range_count;
} typedef binomialrangedata;

#define START_BRL(range, i, j, o, p) \
  o = range.order_min; for (size_t i = 0; o < range.order_max; i++) { \
  p = range.p_min; for (size_t j = 0; p < range.p_max; j++) {

#define END_BRL(range, i, j, o, p) \
  p += range.p_step;}\
  o += range.order_step;}

void sample_range(binomialrangedata s);

void sample_point(sampledata params, sample* out);

#endif 