Various programs to compute the average shortest path in an unweighted
graph.

Program    | Description
---------- | -----------
`casp`     | C version using memoised iterative deepening
`caspv`    | C version using memoised iterative deepening and Intel AVX vectors
`casp-bfs` | C verison using repeated breadth-first search
`hasp`     | Haskell version using memoised iterative deepening
`hasp-bfs` | Haskell version using repeated breadth-first search

Here is a draft paper we've written that describes the programs:
[Functional Pearl: The unweighted average shortest path](asp-draft.pdf).

The networks included in this repo were provided by the authors of the
paper [Language and Hardware Acceleration Backend for Graph
Processing](https://github.com/tuura/papers/tree/master/fdl-2017).
