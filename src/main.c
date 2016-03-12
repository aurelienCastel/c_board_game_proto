#include <stdio.h>

#include "console_color.h"
#include "token_types.h"
#include "heroes.h"
#include "player.h"
#include "board.h"

int main(void)
{
	struct board board;
	init_board(&board, 8, (char*[]){"*  *****",
								    "  ****",
								    "* ** **",
								    "*  *** * *",
								    "  **** **",
									"*** * * *",
									"    ***",
									"   **"});

	printf("(row_nb) %d\n", board.row_nb);

	int y, x;

	for(y = 0; y < board.row_nb; y++)
		printf("(row_info %d) limit_left: %d, limit_right: %d\n", y,
				board.row_info[y].limit_left, board.row_info[y].limit_right);

	for(y = 0; y < board.row_nb; y++)
	{
		for(x = board.row_info[y].limit_left; x <= board.row_info[y].limit_right; x++)
		{
			if(!board.grid[y][x].is_hole)
				printf("%d|%d -> %s\n", y, x, board.grid[y][x].is_border ? "border":"not border");
		}
	}

	print_board(&board);

	delete_board(board);

	return 0;
}
