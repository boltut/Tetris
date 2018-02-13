tetris: main.o drawer.o model.o
	g++  -o tetris main.o drawer.o model.o  -lSDL2

main.o: main.cpp drawer.h
	g++ main.cpp drawer.h -std=c++17 -c 

drawer.o: drawer.cpp drawer.h
	g++ drawer.cpp drawer.h -std=c++17 -c

model.o:  model.cpp model.h
	g++ model.cpp model.h -std=c++17 -c

clean:
	rm  tetris *.o *.gch