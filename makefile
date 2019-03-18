CC=g++
CPPFLAGS=-Wall -pedantic
HPATH=./
SRCPATH=./
OBJPATH=./

__start__: program.out
	./program.out

program.out: $(OBJPATH)main.o $(OBJPATH)Window.o
	$(CC) -o $@ $^ -lGLEW -lglfw -lGL

$(OBJPATH)main.o: $(SRCPATH)main.cpp $(HPATH)Window.h
	$(CC) -c -o $@ $< $(CPPFLAGS) -lGLEW -lglfw -lGL

$(OBJPATH)Window.o: $(SRCPATH)Window.cpp $(HPATH)Window.h
	$(CC) -c -o $@ $< $(CPPFLAGS) -lGLEW -lglfw -lGL

.PHONY: clean

clean:
	rm -f $(OBJPATH)*.o
	rm program.out