all: hasp hasp-bfs hasp-bfs2

hasp: *.hs 
	ghc -cpp --make -O2 Main.hs -o hasp

hasp-bfs: *.hs 
	ghc -DUSE_BFS -cpp --make -O2 Main.hs -o hasp-bfs

hasp-bfs2: *.hs 
	ghc -DUSE_BFS2 -cpp --make -O2 Main.hs -o hasp-bfs2

clean:
	rm -f hasp hasp-bfs hasp-bfs2 *.o *.hi
