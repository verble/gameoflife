
#ifndef BOARD_GRAPHICS_H
#define BOARD_GRAPHICS_H

#include "board.h"


GLfloat* generate_pos_buffer(Board b);

GLfloat* generate_col_buffer(Board b);

typedef struct BoardGraphics {
    Board board;
    GLfloat* positions;
    GLfloat* colors;
    ShaderProgram program;
    GLuint vao;
    GLuint position_buffer;
    size_t position_buffer_size;
    GLuint color_buffer;
    size_t color_buffer_size;
} BoardGraphics;

void board_graphics_destroy(BoardGraphics g);

void board_graphics_update(BoardGraphics* g, Board b);

void board_graphics_draw(BoardGraphics g);

BoardGraphics board_graphics_create(Board b);

#endif // BOARD_GRAPHICS_H
