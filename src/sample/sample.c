#include <sample/sample.h>
#include <stddef.h>
#include <combinations/xoshiro.h>
#include <graph/graph.h>

// Function to calculate the edge count of a graph
size_t property(NeighborhoodGraph g) {
    return g.edges;
}

// Thread function to perform partial sampling
void sample_point(sampledata params, sample* out) {

    binomial_graph_random_sample(params, out, property);

    double mean = 0.0;
    for (int i = 0; i < out->len; i++) {
      //printf("\t\tout (n:%ld, p:%f): %ld\n", t_data->order, t_data->p, out[i]);
      mean += (double)(out->x)[i] / (double)(out->len);
    }
    out->mean = mean;
}

void sample_range(binomialrangedata s) {

  binomialrange r = s.range;
  size_t vertical_steps = (r.order_max - r.order_min) / r.order_step;
  size_t horizontal_steps = (r.p_max - r.p_min) / r.p_step;
  size_t table_size = vertical_steps * horizontal_steps;

  sXSRPA paramA = { XSR_256, XSR_RANDOM_ALL, 1 };  // Choose configuration
  sXSRPB paramB = { XSR_RANDOM_SM, 1 };
  pXSR random_state = fnAllocXSR(s.seed, paramA, paramB);

  
  //printf("step sizes: %lu %f\n", total_range.order_step, total_range.p_step);

  size_t* out = (size_t*)calloc(table_size, sizeof(size_t));
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
    sample_point(data, &sample);
    //printf("mean: %lf\n", s.mean);
    out[i] += sample.mean / (double)s.range_count;
  }

  fnDelocXSR(random_state);
  free(sample_register);
  free(out);
}