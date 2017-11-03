all: casp caspv hasp

casp:
	make -C C casp
	cp C/casp .

caspv: 
	make -C C caspv
	cp C/caspv .

hasp:
	make -C Haskell hasp
	cp Haskell/hasp .

clean:
	make -C Haskell clean
	make -C C clean
	rm -rf casp caspv hasp
