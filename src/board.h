#ifndef board_h
#define board_h

#include <stdint.h>

// I don't think this structure need to be stored on the heap, there is nothing to free in it.
// There it doesn't need to returns a pointer.

// There isn't even a need for a creator function, the struct can be initialized with {.a = ..., .c = ...}.
// So there is no need to delete it or something.
struct cell
{
	uint8_t is_hole;
	uint8_t is_border;
	struct token* token;
};

struct row_info
{
	uint8_t limit_left;
	uint8_t limit_right;
};

struct board
{
	struct cell** grid;
	uint8_t row_nb;
	struct row_info* row_info;
};

void init_board(struct board* board, uint8_t row_nb, char* model[]);
void delete_board(struct board board);

void print_board(struct board* board);

#endif
