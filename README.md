# GraphC

The code powering my final project for Graph Theory at school. It primarily computes a value on a large sample of random graphs, using sampling distributions and ranges.

There are 5 super-lightweight libraries at play here:
 - graph, with the `NeighborhoodGraph` type for directed graphs and the `ShortestPathGraph` type for representing shortest paths.
 - choose, with the `choose_i` function for enumerating $k$-tuples on $[n]$ and `choose` for computing ${n \choose k}$.
 - sample, with functions for generically sampling ranges of random graphs.
 - [sqlite3](https://www.sqlite.org/capi3ref.html) and sqlite3_helpers, with functions for generically executing batch sqlite3 transactions
 - [xoshiro](https://github.com/Lima-X/XSRlib)

# To run with default settings

Pull the code and run `cmake`, then run `make`. Then, to visualize the output, run `main.py` with `python3`. 

I haven't tested this outside of my own laptop on WSL and my school's servers (Ubuntu), so open an issue if it doesn't work for you! Especially for non-unix users.

# To modify parameters

The size of the sample is parametrized by the define directives in samplingdistribution.c and samplingdistribution.h, including but not limited to the number of threads, the number of samples to make at each point in the range on each thread, and the range in terms of min order, max order, order step, min probability, and so on. Change these definitions and recompile to use them.
