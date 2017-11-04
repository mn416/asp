all: casp caspv casp-bfs

casp: casp.c
	gcc -O3 casp.c -mpopcnt -o casp

casp-bfs: casp-bfs.c
	gcc -O3 casp-bfs.c -mpopcnt -o casp-bfs

caspv: caspv.c
	gcc -O3 caspv.c -mavx -mpopcnt -o caspv

clean:
	rm -f casp caspv casp-bfs
