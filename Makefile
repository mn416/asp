all: casp caspv casph casp-bfs hasp hasp-bfs

casp:
	make -C C casp
	cp C/casp .

caspv: 
	make -C C caspv
	cp C/caspv .

casph: 
	make -C C casph
	cp C/casph .

casp-bfs: 
	make -C C casp-bfs
	cp C/casp-bfs .

hasp:
	make -C Haskell hasp
	cp Haskell/hasp .

hasp-bfs:
	make -C Haskell hasp-bfs
	cp Haskell/hasp-bfs .

clean:
	make -C Haskell clean
	make -C C clean
	rm -rf casp casph caspv casp-bfs hasp hasp-bfs
