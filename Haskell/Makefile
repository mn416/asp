all: hasp hasp-bfs hasp-bfs2

hasp: *.hs 
	ghc -D_ALG_=MID -cpp --make -O2 Main.hs -o hasp

hasp-bfs: *.hs 
	ghc -D_ALG_=BFS -cpp --make -O2 Main.hs -o hasp-bfs

hasp-bfs2: *.hs 
	ghc -D_ALG_=BFS2 -cpp --make -O2 Main.hs -o hasp-bfs2

hasp-fgl: *.hs 
	ghc -D_ALG_=FGL -cpp --make -O2 Main.hs -o hasp-fgl

clean:
	rm -f hasp hasp-bfs hasp-bfs2 hasp-fgl *.o *.hi
