#include <sample/sample.h>
#include <stddef.h>
#include <combinations/xoshiro.h>
#include <graph/graph.h>
#include <stdio.h>
#include <math.h>

// Thread function to perform partial sampling
void sample_point(sampledata params, sample* out, size_t (*property)(NeighborhoodGraph g)) {

    binomial_graph_random_sample(params, out, property);

    double mean = 0.0;
    for (int i = 0; i < out->len; i++) {
      //printf("\t\tout (n:%ld, p:%f): %ld\n", t_data->order, t_data->p, out[i]);
      mean += (double)(out->x)[i] / (double)(out->len);
    }
    out->mean = mean;
}

void sample_range(binomialrangedata s, double* out, size_t (*property)(NeighborhoodGraph g)) {

  binomialrange r = s.range;
  size_t vertical_steps = (r.order_max - r.order_min) / r.order_step;
  size_t horizontal_steps = (r.p_max - r.p_min) / r.p_step;
  size_t table_size = vertical_steps * horizontal_steps;

  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(s.seed, paramA, paramB);

  memset(out, 0, sizeof(double) * s.sample_count);
  
  //printf("step sizes: %lu %f\n", total_range.order_step, total_range.p_step);

  size_t* sample_register = malloc(s.sample_count * sizeof(size_t));
  sample sample = {sample_register, s.sample_count, 0.0};

  size_t stride = (r.p_max - r.p_min) / r.p_step;
  
  for (int i = 0; i < table_size; i++) {
    sampledata data = {
      r.order_min + i / stride * r.order_step, 
      r.p_min + i % stride * r.p_step, 
      s.sample_count,
      random_state, 
    };
    sample_point(data, &sample, property);
    out[i] = sample.mean;
  }

  fnDelocXSR(random_state);
  free(sample_register);
}

size_t get_row(binomialrange r, size_t i) {
  return r.order_min + (i / R_STRIDE(r)) * r.order_step;
}

float get_col(binomialrange r, size_t i) {
  return r.p_min + (i % (size_t)R_STRIDE(r)) * r.p_step;
}