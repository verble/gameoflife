
INCLUDE_PATHS = -I/usr/local/include
LIBRARY_PATHS = -L/usr/local/lib
LINKER_FLAGS = -framework OpenGL -lglfw3 -lglew

all: test

test: window.o
	clang -o test window.o $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

window.o: window.c
	clang -c window.c

clean:
	rm -f test
	rm -f *.o

run: test
	./test
