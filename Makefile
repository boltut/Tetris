OBJECTS = main.o drawer.o model.o

tetris: $(OBJECTS)
	g++ -o tetris $(OBJECTS) -lSDL2 -lSDL2_ttf
main.o drawer.o: drawer.h
	g++ -g3 main.cpp drawer.cpp -std=c++17 -c
model.o: model.h infopanel.h
	g++ -g3 model.cpp -std=c++17 -c
clean:
	rm tetris $(OBJECTS) *.gch