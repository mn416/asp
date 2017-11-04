all: casp caspv casp-bfs hasp

casp:
	make -C C casp
	cp C/casp .

caspv: 
	make -C C caspv
	cp C/caspv .

casp-bfs: 
	make -C C caspv-bfs
	cp C/caspv-bfs .

hasp:
	make -C Haskell hasp
	cp Haskell/hasp .

clean:
	make -C Haskell clean
	make -C C clean
	rm -rf casp caspv casp-bfs hasp
