Computes the average shortest path on a PC, for graphs with high
connectivity and low diameter.

On an `Intel(R) Core(TM) i7-6770HQ CPU @ 2.60GHz`:

```
> make
gcc -O3 asp-scalar.c -mpopcnt -o asp-scalar
gcc -O3 asp-vector.c -mavx -mpopcnt -o asp-vector

> ./asp-scalar n4.edges
Sum of shortest paths = 6759254
Time = 0.004995

> ./asp-vector n4.edges
Sum of shortest paths = 6759254
Time = 0.003320

> ./asp-scalar n5.edges
Sum of shortest paths = 31710430
Time = 0.024734

> ./asp-vector n5.edges
./asp-vector n5.edges 
Sum of shortest paths = 31710430
Time = 0.013122
```
