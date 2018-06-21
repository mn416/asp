all: casp casph caspv casp-bfs

casp: casp.c
	gcc -std=c11 -D_DEFAULT_SOURCE -O3 casp.c -mpopcnt -o casp

casph: casph.c
	gcc -std=c11 -D_DEFAULT_SOURCE -O3 casph.c -mpopcnt -o casph

casp-bfs: casp-bfs.c
	gcc -std=c11 -D_DEFAULT_SOURCE -O3 casp-bfs.c -mpopcnt -o casp-bfs

caspv: caspv.c
	gcc -std=c11 -D_DEFAULT_SOURCE -O3 caspv.c -mavx -mpopcnt -o caspv

clean:
	rm -f casp casph caspv casp-bfs
