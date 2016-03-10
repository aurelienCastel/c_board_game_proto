#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include<stdint.h>

#include "board.h"
#include "util.h"

// There is a way model should be write, return error if it doesnt correspond.
void init_board(struct board* board, uint8_t row_nb, char* model[])
{
	uint8_t x, y;

	board->row_nb = row_nb;

	board->grid = malloc(row_nb * sizeof *(board->grid));
	board->row_info = malloc(row_nb * sizeof *(board->row_info));

	for(y = 0; y < row_nb; y++)
	{
		board->grid[y] = malloc(string_length(model[y]) * sizeof **(board->grid));
		board->row_info[y].limit_left = 0;
		board->row_info[y].limit_right = 0;

		for(x = 0; model[y][x] == ' '; x++)
			board->row_info[y].limit_left++;
		board->row_info[y].limit_right = board->row_info[y].limit_left - 1;
		for(; model[y][x] != '\0'; x++)
		{
			if(model[y][x] == '*')
			{
				board->grid[y][x].token = NULL;
				board->grid[y][x].is_hole = 0;
			}
			else if(model[y][x] == ' ')
				board->grid[y][x].is_hole = 1;
			//else error
			board->row_info[y].limit_right++;
		}
	}

	for(y = 0; y < row_nb; y++)
	{
		for(x = board->row_info[y].limit_left;
				x <= board->row_info[y].limit_right; x++)
		{
			if(!board->grid[y][x].is_hole)
			{
				if(y == 0 || y == row_nb - 1 ||
				   x == board->row_info[y].limit_right || x == board->row_info[y].limit_left || 
				   x < board->row_info[y - 1].limit_left || x > board->row_info[y - 1].limit_right ||
				   x < board->row_info[y + 1].limit_left || x > board->row_info[y + 1].limit_right ||
				   board->grid[y - 1][x].is_hole || board->grid[y][x - 1].is_hole ||
				   board->grid[y + 1][x].is_hole || board->grid[y][x + 1].is_hole) 
					board->grid[y][x].is_border = 1;
				else
					board->grid[y][x].is_border = 0;
			}
		}
	}
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
void delete_board(struct board board)
{
	uint8_t y;

	free(board.row_info);
	for(y = 0; y < board.row_nb; y++)
		free(board.grid[y]);
	free(board.grid);
}

void print_board(struct board* board)
{
	uint8_t y, x;
	uint8_t left, right;

	printf("\n");
	for(x = 0; x < board->row_info[0].limit_left; x++)
		printf("    ");
	for(; x <= board->row_info[0].limit_right; x++)
	{
		if(!board->grid[0][x].is_hole)
			printf("----");
		else
			printf("    ");
	}

	for(y = 0; y < board->row_nb; y++)
	{
		printf("\n");
		for(x = 0; x < board->row_info[y].limit_left; x++)
			printf("    ");
		printf("|");
		for(; x <= board->row_info[y].limit_right; x++)
		{
			if(!board->grid[y][x].is_hole || !board->grid[y][x + 1].is_hole)
				printf("   |");
			else
				printf("    ");
		}

		if(y == board->row_nb - 1)
			break;

		left = board->row_info[y].limit_left < board->row_info[y + 1].limit_left ? y : y + 1;
		right = board->row_info[y].limit_right > board->row_info[y + 1].limit_right ? y : y + 1;

		printf("\n");
		for(x = 0; x < board->row_info[left].limit_left; x++)
			printf("    ");
		for(; x <= board->row_info[right].limit_right; x++)
		{
			if((x >= board->row_info[y].limit_left &&
				x <= board->row_info[y].limit_right &&
				!board->grid[y][x].is_hole) ||
			   (x >= board->row_info[y + 1].limit_left &&
				x <= board->row_info[y + 1].limit_right &&
				!board->grid[y + 1][x].is_hole))
				printf("----");
			else
				printf("    ");
		}
	}

	printf("\n");
	for(x = 0; x < board->row_info[y].limit_left; x++)
		printf("    ");
	for(; x <= board->row_info[y].limit_right; x++)
	{
		if(!board->grid[y][x].is_hole)
			printf("----");
		else
			printf("    ");
	}
	printf("\n");
}
