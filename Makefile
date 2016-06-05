
INCLUDE_PATHS = -I/usr/local/include
LIBRARY_PATHS = -L/usr/local/lib
LINKER_FLAGS = -framework OpenGL -lglfw3 -lglew

all: test

test: window.o shader.o board.o board_graphics.o millitime.o
	clang -o test window.o shader.o board.o board_graphics.o millitime.o $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)

window.o: window.c
	clang -c window.c

shader.o: shader.c
	clang -c shader.c

board.o: board.c
	clang -c board.c

board_graphics.o: board_graphics.c
	clang -c board_graphics.c

millitime.o: millitime.c
	clang -c millitime.c

clean:
	rm -f test
	rm -f *.o

run: test
	./test
