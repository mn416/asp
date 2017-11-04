all: hasp hasp-bfs

hasp: *.hs 
	ghc -cpp --make -O2 Main.hs -o hasp

hasp-bfs: *.hs 
	ghc -DUSE_BFS -cpp --make -O2 Main.hs -o hasp-bfs

clean:
	rm -f hasp hasp-bfs *.o *.hi
