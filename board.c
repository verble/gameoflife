

#include "board.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


const char* WHITE_CIRCLE = "\u25CB";
const char* BLACK_CIRCLE = "\u25CF";

const Board ERROR_BOARD = { 0, 0, NULL, NULL };


Board board_generate(int width, int height, int initialize, char* data) {

    // width and height must be valid
    if (width < 0 || height < 0) {
        puts("Error: width or height not greater than 0");
        return ERROR_BOARD;
    }

    // size of data must be equal to cell count
    int cell_count = width * height;
    if (initialize && (strlen(data) != cell_count)) {
        puts("Error: length of data is not compatible with board size");
        return ERROR_BOARD;
    }

    size_t mem_length = cell_count * sizeof(CellState);
    CellState* active  = malloc(mem_length);
    CellState* working = malloc(mem_length);

    if (initialize) {
        CellState* cells = string_to_cellstate_array(data);
        for (int i = 0; i < cell_count; i++) {
            active[i] = cells[i];
        }
        free(cells);
    } else {
        for (int i = 0; i < cell_count; i++) {
            active[i] = CELL_STATE_DEAD;
        }
    }

    Board b = { width, height, active, working };
    return b;
}

void board_destroy(Board b) {
    free(b.active);
    free(b.working);
}

void board_evolve(Board* b) {

    for (int x = 0; x < (*b).width; x++) {
        for (int y = 0; y < (*b).width; y++) {
            CellState new_state = evolve_cell(*b, x, y);
            int offset = get_offset(*b, x, y);
            (*b).working[offset] = new_state;
        }
    }

    // swap boards
    CellState* temp;
    temp = (*b).active;
    (*b).active = (*b).working;
    (*b).working = temp;
}

CellState board_get_cell(Board b, int x, int y) {
    int offset = get_offset(b, x, y);
    return b.active[offset];
}

void board_set_cell(Board b, int x, int y, CellState val) {
    int offset = get_offset(b, x, y);
    b.active[offset] = val;
}

void board_print(Board b) {
    for (int y = 0; y < b.height; y++) {
        for (int x = 0; x < b.width; x++) {
            CellState s = b.active[(b.width * y) + x];
            if (s == CELL_STATE_ALIVE) {
                printf("%s", BLACK_CIRCLE);
            } else {
                printf("%s", WHITE_CIRCLE);
            }
        }
        printf("\n");
    }
}


/* ---- PRIVATE API ---- */


CellState* string_to_cellstate_array(char *str) {
    int len = strlen(str);
    CellState* cells = malloc(len * sizeof(CellState));

    // all values not equal to 1 are set to CELL_STATE_DEAD
    for (int i = 0; i < len; i++) {
        if (str[i] == '1') {
            cells[i] = CELL_STATE_ALIVE;
        } else {
            cells[i] = CELL_STATE_DEAD;
        }
    }

    return cells;
}

CellState evolve_cell(Board b, int x, int y) {
    CellState neighbors[8];
    get_neighbors(b, x, y, neighbors);

    int neighbor_count = 0;
    for (int i = 0; i < 8; i++) {
        if (neighbors[i] == CELL_STATE_ALIVE) {
            neighbor_count++;
        }
    }

    if (neighbor_count < 2) {
        // under-population
        return CELL_STATE_DEAD;
    } else if (neighbor_count > 3) {
        // over-population
        return CELL_STATE_DEAD;
    } else if (neighbor_count == 3) {
        // birth
        return CELL_STATE_ALIVE;
    } else {
        // no change
        return board_get_cell(b, x, y);
    }
}

void get_neighbors(Board b, int x, int y, CellState* neighbors) {
    neighbors[0] = board_get_cell(b, x, y-1);   // N
    neighbors[1] = board_get_cell(b, x+1, y-1); // NE
    neighbors[2] = board_get_cell(b, x+1, y);   // E
    neighbors[3] = board_get_cell(b, x+1, y+1); // SE
    neighbors[4] = board_get_cell(b, x, y+1);   // S
    neighbors[5] = board_get_cell(b, x-1, y+1); // SW
    neighbors[6] = board_get_cell(b, x-1, y);   // W
    neighbors[7] = board_get_cell(b, x-1, y-1); // NW
}

int get_offset(Board b, int x, int y) {
    int xwrapped = (x % b.width);
    xwrapped = (xwrapped < 0) ? xwrapped + b.width : xwrapped;
    int ywrapped = (y % b.height);
    ywrapped = (ywrapped < 0) ? ywrapped + b.height : ywrapped;
    return (b.width * ywrapped) + xwrapped;
}
