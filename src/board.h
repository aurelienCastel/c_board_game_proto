#ifndef board_h
#define board_h

#include <stdint.h>
#include "player.h"

struct coord
{
	uint8_t x;
	uint8_t y;
};

struct cell
{
	uint8_t is_hole;
	uint8_t is_border;
	struct player* border_check_against[2];
	struct player* align_check_against[2];
	struct player* token;
};

struct board_model
{
	char* name;
	uint8_t nb_rows;
	char** model;
};

struct board
{
	char* name;
	struct cell** grid;
	uint8_t height;
	uint8_t length;
};

void init_board(struct board* board, struct board_model* model);
void delete_board(struct board board);

void print_board(struct board* board);

#endif
