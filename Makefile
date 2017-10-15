all: asp-scalar asp-vector

asp-scalar: asp-scalar.c
	gcc -O3 asp-scalar.c -mpopcnt -o asp-scalar

asp-vector: asp-vector.c
	gcc -O3 asp-vector.c -mavx -mpopcnt -o asp-vector

clean:
	rm -f asp-scalar asp-vector
