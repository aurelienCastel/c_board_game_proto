#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

#include "board.h"
#include "console_color.h"
#include "util.h"

uint8_t is_border(struct board* board, uint8_t y, uint8_t x)
{
	if(y == 0 || y == board->height - 1 ||
	   x == 0 || x == board->length - 1 || 
	   board->grid[y - 1][x].is_hole || board->grid[y][x - 1].is_hole ||
	   board->grid[y + 1][x].is_hole || board->grid[y][x + 1].is_hole) 
		return 1;
	return 0;
}

void init_board(struct board* board, struct board_model* model)
{
	uint8_t x, y;

	board->name = string_copy(model->name, string_length(model->name));

	board->height = model->nb_rows;
	board->length = longest_string(model->model, model->nb_rows);

	board->grid = malloc(board->height * sizeof *(board->grid));

	for(y = 0; y < board->height; y++)
	{
		board->grid[y] = malloc(board->length * sizeof **(board->grid));

		for(x = 0; model->model[y][x] != '\0'; x++)
		{
			board->grid[y][x].token = NULL;
			board->grid[y][x].check_against[0] = NULL;
			board->grid[y][x].check_against[1] = NULL;

			if(model->model[y][x] == '*')
				board->grid[y][x].is_hole = 0;
			else
				board->grid[y][x].is_hole = 1;
		}
	}

	for(y = 0; y < board->height; y++)
	{
		for(x = 0; x < board->length; x++)
		{
			if(!board->grid[y][x].is_hole)
				board->grid[y][x].is_border = is_border(board, y, x);
		}
	}
}

void delete_board(struct board board)
{
	uint8_t y;

	free(board.name);
	for(y = 0; y < board.height; y++)
		free(board.grid[y]);
	free(board.grid);
}

void print_board(struct board* board)
{
	uint8_t y, x;
	uint8_t left, right;

	printf("\n");
	printf("\t");
	for(x = 0; x < board->length; x++)
	{
		if(board->grid[0][x].is_hole)
			printf("    ");
		else
			printf("----");
	}

	for(y = 0; y < board->height; y++)
	{
		printf("\n");
		printf("\t");
		for(x = 0; x < board->length; x++)
		{
			if(board->grid[y][x].is_hole)
			{
				printf("   ");
				if(x < board->length - 1 && board->grid[y][x + 1].is_hole)
					printf(" ");
			}
			else
			{
				printf("|");
				if(board->grid[y][x].token == NULL)
					cc_fprintf(BG_DARK_YELLOW, stdout, "   ");
				else
					cc_fprintf(board->grid[y][x].token->color, stdout, " O ");
				printf("|");
			}
		}

		printf("\n");
		printf("\t");
		for(x = 0; x < board->length; x++)
		{
			if(board->grid[y][x].is_hole &&
			   (y == board->height - 1 || board->grid[y + 1][x].is_hole))
				printf("    ");
			else
				printf("----");
		}
	}
	printf("\n");
}
