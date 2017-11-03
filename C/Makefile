all: casp caspv

casp: asp-scalar.c
	gcc -O3 asp-scalar.c -mpopcnt -o casp

caspv: asp-vector.c
	gcc -O3 asp-vector.c -mavx -mpopcnt -o caspv

clean:
	rm -f casp caspv
