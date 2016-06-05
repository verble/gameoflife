

#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>


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
