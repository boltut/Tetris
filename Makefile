OBJECTS = main.o drawer.o model.o

tetris: $(OBJECTS)
	g++ -o tetris $(OBJECTS) -lSDL2
main.o drawer.o: drawer.h
	g++ main.cpp drawer.cpp -std=c++17 -c 
model.o: model.h
	g++ model.cpp -std=c++17 -c
clean:
	rm tetris $(OBJECTS) *.gch