

#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "shader.h"
#include "board_graphics.h"


// 2 triangles per square * 3 vertices per triangle
const int VERTICES_PER_CELL = 2 * 3;
// 2 triangles per square * 3 vertices per triangle * 2 coords per vertex
const int POS_SEGMENT_SIZE = 2 * 3 * 2;
// 2 triangles per square * 3 vertices per triangle * 3 color points per vertex
const int COL_SEGMENT_SIZE = 2 * 3 * 3;

// the position buffer is organized by cell
// (grouped by row, rows increasing from 0)
// for each cell there are two triangles
// the triangle vertices are listed in the order
// NW, NE, SW, NE, SE, SW
// and each vertex has two coordinates in (x,y) order
GLfloat* generate_pos_buffer(Board b) {

    int element_count = b.width * b.height * POS_SEGMENT_SIZE;
    GLfloat* buffer = malloc(element_count * sizeof(GLfloat));

    // generate a grid of squares
    for (int y = 0; y < b.height; y++) {
        for (int x = 0; x < b.width; x++) {
            int offset = (y * b.width * POS_SEGMENT_SIZE) + (x * POS_SEGMENT_SIZE);

            int coordinates[] = {
                x,    y,    // NW  left triangle
               (x+1), y,    // NE
                x,   (y+1), // SW
               (x+1), y,    // NE  right triangle
               (x+1),(y+1), // SE
                x,   (y+1)  // SW
            };

            for (int i = 0; i < POS_SEGMENT_SIZE; i++) {
                buffer[offset + i] = coordinates[i];
            }
        }
    }

    // scale the grid
    // if you have x cells going across, the max x coord produced above
    // will be x, and likewise for y going down
    // the scaling factor is (2/coords) because the viewport goes from
    // -1.0 to 1.0. after the scale values range from 0 to 2, so
    // then you subtract 1.0 to get a range from -1 to 1
    float scalex = 2 / (float) b.width;
    float scaley = 2 / (float) b.height;

    // scale x coords
    for (int i = 0; i < element_count; i += 2) {
        buffer[i] *= scalex;
        buffer[i] -= 1.0;
    }

    // scale y coords
    for (int i = 1; i < element_count; i += 2) {
        buffer[i] *= scaley;
        buffer[i] -= 1.0;
        // flip y axis to account for GL viewport coordinate system
        buffer[i] *= -1.0;
    }

    return buffer;
}

// the color buffer is organized by cell
// (grouped by row, rows increasing from 0)
// each cell has 6 vertices of the same color
GLfloat* generate_col_buffer(Board b) {

    int element_count = b.width * b.height * COL_SEGMENT_SIZE;
    GLfloat* buffer = malloc(element_count * sizeof(GLfloat));

    for (int y = 0; y < b.height; y++) {
        for (int x = 0; x < b.width; x++) {
            int offset = (y * b.width * COL_SEGMENT_SIZE) + (x * COL_SEGMENT_SIZE);

            for (int i = 0; i < COL_SEGMENT_SIZE; i++) {
                if (board_get_cell(b, x, y) == CELL_STATE_ALIVE) {
                    buffer[offset + i] = 0.0;
                } else {
                    buffer[offset + i] = 1.0;
                }
            }
        }
    }

    return buffer;
}




// OpenGL must be ready to go
BoardGraphics board_graphics_create(Board b) {

    BoardGraphics g;
    g.board = b;
    g.positions = generate_pos_buffer(b);
    g.colors = generate_col_buffer(b);
    g.program = shprog_new("vertex_shader.glsl", "fragment_shader.glsl");
    g.position_buffer_size =
        g.board.width * g.board.height * POS_SEGMENT_SIZE * sizeof(GLfloat);
    g.color_buffer_size =
        g.board.width * g.board.height * COL_SEGMENT_SIZE * sizeof(GLfloat);

    // vertex array object
    glGenVertexArrays(1, &(g.vao));

    // vertex buffers
    glGenBuffers(1, &(g.position_buffer));
    glGenBuffers(1, &(g.color_buffer));

    // copy data
    glBindVertexArray(g.vao);
    glBindBuffer(GL_ARRAY_BUFFER, g.position_buffer);
    glBufferData(GL_ARRAY_BUFFER, g.position_buffer_size, g.positions, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, g.color_buffer);
    glBufferData(GL_ARRAY_BUFFER, g.color_buffer_size, g.colors, GL_STREAM_DRAW);

    // register attributes
    glBindBuffer(GL_ARRAY_BUFFER, g.position_buffer);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, g.color_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    return g;
}

void board_graphics_draw(BoardGraphics g) {
    shprog_use(g.program);
    glBindVertexArray(g.vao);
    glDrawArrays(GL_TRIANGLES, 0, (g.board.width * g.board.height * VERTICES_PER_CELL));
    glBindVertexArray(0);
}

void board_graphics_update(BoardGraphics* g, Board b) {
    (*g).board = b;
    free((*g).colors);
    (*g).colors = generate_col_buffer(b);
    glBindBuffer(GL_ARRAY_BUFFER, (*g).color_buffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, (*g).color_buffer_size, (*g).colors);
}

// does NOT free the board
void board_graphics_destroy(BoardGraphics g) {

    glDeleteVertexArrays(1, &(g.vao));
    glDeleteBuffers(1, &(g.position_buffer));
    glDeleteBuffers(1, &(g.color_buffer));
    glDeleteProgram(g.program);

    free(g.positions);
    free(g.colors);
}
