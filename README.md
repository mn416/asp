Computes the average shortest path on a PC, for graphs with high
connectivity and low diameter.

The various programs are:

Program    | Description
---------- + -----------
`casp`     | C version using memoised iterative deepening
`caspv`    | C version using memoised iterative deepening and Intel AVX vectors
`casp-bfs` | C verison using repeated breadth-first search
`hasp`     | Haskell version using memoised iterative deepening

On an `Intel(R) Core(TM) i7-6770HQ CPU @ 2.60GHz`:

```
> make

### Network n4 ###

> ./caspv Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.003320

> ./casp Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.004995

> ./hasp Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.0240s

> ./casp-bfs Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.113536

### Network n5 ###

> ./caspv Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.013122

> ./casp Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.024734

> ./hasp Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.0920s

> ./casp-bfs Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.580013
```
