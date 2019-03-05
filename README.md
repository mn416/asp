# Computing the average shortest path

This repo provides various programs to compute the [average shortest
path](https://en.wikipedia.org/wiki/Average_path_length) in an
unweighted graph, along with some benchmark graphs, and performance
results.  The main idea (*memoised iterative deepening*, which we now
know is a variant of the [Bellman-Ford
algorithm](https://en.wikipedia.org/wiki/Bellman%E2%80%93Ford_algorithm)
specialised to unweighted graphs) is described and benchmarked in the
following draft paper:

[Functional Pearl: The unweighted average shortest path](asp-draft.pdf)

The graphs included in this repo were provided by the authors of the
paper [Language and Hardware Acceleration Backend for Graph
Processing](https://github.com/tuura/papers/tree/master/fdl-2017).

All the programs in this repo are single-threaded, and likely to
benifit significantly from multithreading.  The programs are:

Program    | Description
---------- | -----------
`casp`     | C version using memoised iterative deepening
`caspv`    | C version using memoised iterative deepening and Intel AVX vectors
`casp-bfs` | C version using repeated breadth-first search
`casph`    | C version using repeated MID hybrid
`hasp`     | Haskell version using memoised iterative deepening
`hasp-bfs` | Haskell version using repeated breadth-first search
