#include <stdio.h>

#include "token_types.h"
#include "heroes.h"
#include "player.h"
#include "board.h"

int main(void)
{
	printf("%s -> %c\n", Standard_type.name, Standard_type.representation);
	printf("%s -> %c\n", Fire_type.name, Fire_type.representation);
	printf("%s | %s -> %c\n", Yacine.name, Yacine.token_type->name, Yacine.token_type->representation);

	struct player player;
	init_player(&player, "Aurelien", 0, 1, 1, "blue", &Yacine);

	printf("%s | %d %d %d %s |  %s %s -> %c\n"\
			"%s %s %s\n", player.name, player.is_ai,
			player.number, player.team, player.color,
			player.hero->name, player.hero->token_type->name, player.hero->token_type->representation,
			player.token.type->name, player.special_token.type->name,
			player.token.owner->name);

	delete_player(player);

	struct board board;
	init_board(&board, 5, (char*[]){"   *",
								    "  ****",
								    "* *****",
								    "*  ***",
								    "  ****"});

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
