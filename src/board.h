#ifndef board_h
#define board_h

#include <stdint.h>

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
	char* name;
	struct cell** grid;
	uint8_t row_nb;
	struct row_info* row_info;
};

struct board_model
{
	char* name;
	uint8_t row_nb;
	char** model;
};


void init_board(struct board* board, struct board_model* model);
void delete_board(struct board board);

void print_board(struct board* board);

#endif
