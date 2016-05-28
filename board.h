
#ifndef BOARD_H
#define BOARD_H

typedef enum CellState {
    CELL_STATE_ALIVE,
    CELL_STATE_DEAD
} CellState;

/*
 *  A board has cutomizable dimensions 'width' and 'height'.
 *  The edges are wrapped around to form a torus. It is
 *  addressed by a coordinate pair (x,y) where (0,0) is
 *  the address of the top-left corner when the board is
 *  displayed in two dimensinons.
 *
 *  Internally it is composed of two memory segments,
 *  'active' and 'working', which are lists of 'CellState's.
 *  The working segment is used when computing the next
 *  generation.
 */
typedef struct Board {
    int width;
    int height;
    CellState* active;
    CellState* working;
} Board;

/*
 *  Creates a new board of size 'width' x 'height'.
 *  If 'initialize' is not 0 the cells will be set to the
 *  values in 'data', where a 1 represents alive and 0
 *  represents dead. If 'initialize' is 0 all cells
 *  will be set to dead.
 */
Board board_generate(int width, int height, int initialize, char* data);

/*
 *  Frees the memory associated with a board.
 */
void board_destroy(Board);

/*
 *  Advances cells to next generation.
 */
void board_evolve(Board* b);

/*
 *  Gets the value of the cell at ('x', 'y').
 *  Coordinates are mapped to the size of the board.
 */
CellState board_get_cell(Board b, int x, int y);

/*
 *  Sets the value of the cell at ('x', 'y') to 'val'.
 *  Coordinates are mapped to the size of the board.
 */
void board_set_cell(Board b, int x, int y, CellState val);

/*
 *  Prints the board to standard out.
 */
void board_print(Board b);


/* ---- PRIVATE API ---- */


/*
 *  Takes a string of 0s and 1s and returns a
 *  pointer to an array of CellStates.
 */
CellState* string_to_cellstate_array(char *str);

/*
 *  Maps coordinates to the memory offset.
 */
int get_offset(Board b, int x, int y);

/*
 *  Fills the supplied 'CellState[8]' array with the
 *  values of the neighbors of cell ('x', 'y').
 */
void get_neighbors(Board b, int x, int y, CellState* neighbors);

/*
 *  Given a cell's coordinates, returns the value
 *  of the cell in the next generation.
 */
CellState evolve_cell(Board b, int x, int y);

#endif // BOARD_H
