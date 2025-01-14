# GraphC

The code powering my final project for Graph Theory at school. It primarily computes a value on a large sample of random graphs, using sampling distributions and ranges.

There are 5 super-lightweight libraries at play here:
 - graph, with the `NeighborhoodGraph` type for directed graphs and the `ShortestPathGraph` type for representing shortest paths.
 - choose, with the `choose_i` function for enumerating $k$-tuples on $[n]$ and `choose` for computing ${n \choose k}$.
 - sample, with functions for generically sampling ranges of random graphs.
 - [sqlite3](https://www.sqlite.org/capi3ref.html) and sqlite3_helpers, with functions for generically executing batch sqlite3 transactions
 - [xoshiro](https://github.com/Lima-X/XSRlib)

This code is for people who are familiar with random graphs, some graph theory, and some C and data visualization.

# To use

Pull the code and run `cmake`, then run `make`.

If this works, a bunch of text should appear in the terminal and a new file called `out.db` should be generated.

What just happened was a bunch of random graphs were generated, and their components were counted into a sampling distribution. 

This distribution was written to a table in `out.db`.

To add more samples, you need to edit this section of code in `samplingdistribution.c`:

```
#define SAMPLES_PER_THREAD (size_t)10
#define THREAD_COUNT 8

#define RANGE {101, 250, 1, 0.05, 0.1, 0.001};

#define OUT_DB "out.db"
#define OUT_TABLE "samples_9"
#define OUT_COLUMNS {"n", "p", "property"}
#define INITIAL_COLUMN "sample_id"
```

(I might update the code later to work using command-line arguments, but this is a tough parameter set to compress into terminal grammar)

`RANGE` describes, in order, the minimum order, maximum order, order step, min edge probability, max edge probabiliy, and edge probability step. The minimums and maximums describe the range being sampled, and the step size descibes the resolution.

`OUT_TABLE` describes the name of the table. I would recommend filling "table_1" through "table_9" before continuing. These names correspond with literal tables in `out.db`, which will be created if they are missing, and the program will not run if the table already exists. If you want to replace a table, you have to drop it manually from the database before running the code again.

`SAMPLES_PER_THREAD` and `THREAD_COUNT` together describe the sample rate of the program. To get the number of graphs being sampled at each step of order and edge probability, multiply these two numbers together.

After saving these changes, run `make` again and the executable `GraphC` will be created. Run it to add the sample to the database.

Once you have filled the database, run `main.py` to generate charts. There are a many more charts in the code than the ones that display automatically. The code generating these charts are removed. Uncomment these lines to see them.

I haven't tested this outside of my own laptop on WSL and my school's servers (Ubuntu), so open an issue if it doesn't work for you! Especially for non-unix users. 

# Help! It didn't work!
Open an issue. This isn't a big project, so don't worry if the reason it isn't working is your fault or not. I don't have an issue template, but do provide steps to reproduce the issue from a fresh clone of the repo to the best of your ability.

Common issues:
 - Using too many threads can cause the sqlite3 library to print `Killed` to the terminal with no extra notes. If this happens, try to run smaller jobs or use fewer threads.
 - Setting `RANGE` to an invalid value can cause undefined behavior. Please double-check that the maximum is greater than the minimum and the step size is not too large or too close to 0 (which could cause an extremely high resolution if you're not careful).
 - Setting up the CMake and Makefiles are always finicky. CMake errors are the worst. Just open an issue with the error printout if you weren't able to figure it out.

# Performance
On a Ryzen 7 5800HS with 12 threads using WSL, I was able to compute the range {101, 1000, 1, 0, 0.0025, 0.0005} with 10 samples per thread on 12 threads in about 10 minutes. Thats 540,000 large-ish random graphs sampled. Thanks to my own mishandling of the data, this run was made multiple times with similar performance.
