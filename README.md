Computes the average shortest path on a PC, for graphs with high
connectivity and low diameter.

On an `Intel(R) Core(TM) i7-6770HQ CPU @ 2.60GHz`:

```
> make

> ./casp Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.004995

> ./caspv Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.003320

> ./casp-bfs Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.113536

> ./hasp Networks/n4.edges
Sum of shortest paths = 6759254
Time = 0.0240s

> ./casp Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.024734

> ./caspv Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.013122

> ./casp-bfs Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.580013

> ./hasp Networks/n5.edges
Sum of shortest paths = 31710430
Time = 0.0920s
```
