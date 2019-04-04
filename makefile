CC=g++
CPPFLAGS=-Wall -pedantic
HPATH=./
SRCPATH=./
OBJPATH=./
LIBS=-lGLEW -lglfw -lGL

__start__: program.out
	./program.out

program.out: $(OBJPATH)main.o $(OBJPATH)Window.o $(OBJPATH)DrawableObject.o \
				$(OBJPATH)Triangles.o $(OBJPATH)Rect.o $(OBJPATH)Circ.o \
				$(OBJPATH)Curve.o $(OBJPATH)shader.o
	$(CC) -o $@ $^ $(LIBS)

$(OBJPATH)main.o: $(SRCPATH)main.cpp $(HPATH)Window.h $(HPATH)Rect.h \
					$(HPATH)Circ.h $(HPATH)Curve.h $(HPATH)shader.hpp $(HPATH)texture.hpp
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

$(OBJPATH)Window.o: $(SRCPATH)Window.cpp $(HPATH)Window.h
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

$(OBJPATH)DrawableObject.o: $(SRCPATH)DrawableObject.cpp $(HPATH)DrawableObject.h \
							$(HPATH)shader.hpp $(HPATH)vectorUtils.h
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

$(OBJPATH)Triangles.o: $(SRCPATH)Triangles.cpp $(HPATH)Triangles.h \
						$(HPATH)DrawableObject.h 
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

$(OBJPATH)Rect.o: $(SRCPATH)Rect.cpp $(HPATH)Rect.h $(HPATH)Triangles.h
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

$(OBJPATH)Circ.o: $(SRCPATH)Circ.cpp $(HPATH)Circ.h $(HPATH)Triangles.h
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

$(OBJPATH)Curve.o: $(SRCPATH)Curve.cpp $(HPATH)Curve.h $(HPATH)Rect.h $(HPATH)Circ.h
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

$(OBJPATH)shader.o: $(SRCPATH)shader.cpp $(SRCPATH)shader.hpp
	$(CC) -c -o $@ $< $(CPPFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJPATH)*.o
	rm program.out