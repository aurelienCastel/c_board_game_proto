#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "game.h"
#include "util.h"

void init_game(struct game* game, struct player* players[2],
			   struct board* board, uint8_t points_to_win, uint16_t turn_time)
{
	game->players[0] = players[0];
	game->players[1] = players[1];
	game->board = board;
	game->points_to_win = points_to_win;
	game->turn_time = turn_time;
}

void launch_game(struct game* game)
{
	srand(time(NULL));

	uint8_t i, j;
	struct coord coord;

	i = rand() % 2;

	printf("%s play first.\n", game->players[i]->name);

	do
	{
		j = i == 0 ? 1 : 0;

		print_board(game->board);
		coord = move_request(game->board, game->players[i]);
		play_move(game->board, game->players[i], game->players[j], coord.y, coord.x);

		i = j;
	}while(get_winner(game) == NULL);

	printf("The winner of the game is %s.\n", game->players[j]->name);
}

struct coord string_to_coord(char* string)
{
	uint8_t i;
	struct coord coord = {.y = 0, .x = 0};

	for(i = 0; is_digit(string[i]); i++)
	{
		coord.y *= 10;
		coord.y += char_to_int(string[i]);
	}
	if(string[i] == ' ')
		i++;
	for(; is_digit(string[i]); i++)
	{
		coord.x *= 10;
		coord.x += char_to_int(string[i]);
	}

	coord.y -= 1;
	coord.x -= 1;

	return coord;
}

uint8_t cell_is_in(struct board* board, uint8_t y, uint8_t x)
{

	printf("In cell_is_in.\n");

	return y <= board->height && x <= board->length;
}

uint8_t cell_is_playable_for(struct player* player, struct board* board, uint8_t y, uint8_t x)
{ 
	if(!cell_is_in(board, y, x) ||
	   board->grid[y][x].is_hole || board->grid[y][x].token != NULL ||
	   player_is_in(player, board->grid[y][x].border_check_against, 2) ||
	   player_is_in(player, board->grid[y][x].align_check_against, 2))
		return 0;
	return 1;
}

struct coord move_request(struct board* board, struct player* player)
{
	char* input;
	struct coord coord;

	printf("It's %s turn to play.\n", player->name);
	if(!player->is_ai)
	{
		while(1)
		{
			printf("Enter your move:");
			input = get_input(10);
			coord = string_to_coord(input);
			if(!cell_is_playable_for(player, board, coord.y, coord.x)) // Add a different message when the coord is not playable.
				bad_input_message();
			else
				break;
		}
	}
	else
	{
		printf("%s is thinking...\n", player->name);
		// coord = struct coord ai_decision(...)
	}

	return coord;
}

void eliminate_from_to(struct cell** grid, struct player* player, struct player* opponent,
					   struct coord start, struct coord end)
{
	int8_t y_mover = 0;
	int8_t x_mover = 0;
	struct coord chain_pos;

	if(start.y < end.y)
		y_mover = 1;
	if(start.x > end.x)
		x_mover = -1;
	else if(start.x < end.x)
		x_mover = 1;

	printf("In eliminate_from_to.\n");

	printf("y_mover = %d.\n", y_mover);
	printf("x_mover = %d.\n", x_mover);

	while(start.y != end.y || start.x != end.x)
	{
		chain_pos.y = 0;
		chain_pos.x = 0;

		while(grid[start.y][start.x].token != player)
		{
			if(start.y == end.y && start.x == end.x)
				return;

			start.y += y_mover;
			start.x += x_mover;
		}

		while(grid[start.y][start.x].is_hole || grid[start.y][start.x].token == player)
		{
			if(start.y == end.y && start.x == end.x)
				return;

				start.y += y_mover;
				start.x += x_mover;
		}

		if(grid[start.y][start.x].token == opponent)
			chain_pos = start;
		else
			continue;

		while(grid[start.y][start.x].is_hole ||
			  grid[start.y][start.x].token == opponent)
		{
			if(start.y == end.y && start.x == end.x)
				return;

			start.y += y_mover;
			start.x += x_mover;
		}

		if(grid[start.y][start.x].token == player)
		{
				while(chain_pos.y != start.y || chain_pos.x != start.x)
				{
					grid[chain_pos.y][chain_pos.x].token = NULL;
					player->points++;

					chain_pos.y += y_mover;
					chain_pos.x += x_mover;
				}
		}
	}
}

void eliminate_border(struct board* board, struct player* player,
					  uint8_t y, uint8_t x)
{
	printf("In eliminate_border.\n");

	uint8_t opponent_token_around = 0;

	if(cell_is_in(board, y - 1, x) && board->grid[y - 1][x].token == player)
		opponent_token_around++;

	if(cell_is_in(board, y - 1, x + 1) && board->grid[y - 1][x + 1].token == player)
		opponent_token_around++;

	if(cell_is_in(board, y, x + 1) && board->grid[y][x + 1].token == player)
		opponent_token_around++;

	if(cell_is_in(board, y + 1, x + 1) && board->grid[y + 1][x + 1].token == player)
		opponent_token_around++;

	if(cell_is_in(board, y + 1, x) && board->grid[y + 1][x].token == player)
		opponent_token_around++;

	if(cell_is_in(board, y + 1, x - 1) && board->grid[y + 1][x - 1].token == player)
		opponent_token_around++;

	if(cell_is_in(board, y, x - 1) && board->grid[y][x - 1].token == player)
		opponent_token_around++;

	if(cell_is_in(board, y - 1, x - 1) && board->grid[y - 1][x - 1].token == player)
		opponent_token_around++;

	if(opponent_token_around >= 3)
	{
		board->grid[y][x].token = NULL;
		player->points++;
	}
}

void eliminate_borders_around(struct board* board, struct player* player, struct player* opponent,
							  uint8_t y, uint8_t x)
{
	printf("In eliminate_borders_around.\n");

	if(cell_is_in(board, y - 1, x) && board->grid[y - 1][x].is_border &&
	   board->grid[y - 1][x].token == opponent) 
		eliminate_border(board, player, y - 1, x);

	if(cell_is_in(board, y - 1, x + 1) && board->grid[y - 1][x + 1].is_border &&
	   board->grid[y - 1][x + 1].token == opponent) 
		eliminate_border(board, player, y - 1, x + 1);

	if(cell_is_in(board, y, x + 1) && board->grid[y][x + 1].is_border &&
	   board->grid[y][x + 1].token == opponent) 
		eliminate_border(board, player, y, x + 1);

	if(cell_is_in(board, y + 1, x + 1) && board->grid[y + 1][x + 1].is_border &&
	   board->grid[y + 1][x + 1].token == opponent) 
		eliminate_border(board, player, y + 1, x + 1);

	if(cell_is_in(board, y + 1, x) && board->grid[y + 1][x].is_border &&
	   board->grid[y + 1][x].token == opponent) 
		eliminate_border(board, player, y + 1, x);

	if(cell_is_in(board, y + 1, x - 1) && board->grid[y + 1][x - 1].is_border &&
	   board->grid[y + 1][x - 1].token == opponent) 
		eliminate_border(board, player, y + 1, x - 1);

	if(cell_is_in(board, y, x - 1) && board->grid[y][x - 1].is_border &&
	   board->grid[y][x - 1].token == opponent) 
		eliminate_border(board, player, y, x - 1);

	if(cell_is_in(board, y - 1, x - 1) && board->grid[y - 1][x - 1].is_border &&
	   board->grid[y - 1][x - 1].token == opponent) 
		eliminate_border(board, player, y - 1, x - 1);
}

void check_from_to(struct board* board, struct player* player, struct player* opponent,
				   struct coord start, struct coord end)
{
	printf("In check_from_to.\n");

	uint8_t i;
	int8_t y_mover = 0;
	int8_t x_mover = 0;
	struct player* strangleholds[2];
	struct coord cell_pos;

	if(start.y < end.y)
		y_mover = 1;
	if(start.x > end.x)
		x_mover = -1;
	else if(start.x < end.x)
		x_mover = 1;

	while(start.y != end.y || start.x != end.x)
	{
		strangleholds[0] = NULL;
		strangleholds[1] = NULL;

		while(board->grid[start.y][start.x].token == NULL)
		{
			if(board->grid[start.y][start.x].is_border)
				check_border(board, player, opponent, start.y, start.x);
			check_borders_around(board, player, opponent, start.y, start.x);

			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		while(board->grid[start.y][start.x].token != NULL)
		{
			add_player_unique(board->grid[start.y][start.x].token, strangleholds, 2);

			check_borders_around(board, player, opponent, start.y, start.x);

			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		if(board->grid[start.y][start.x].is_border)
			check_border(board, player, opponent, start.y, start.x);
		check_borders_around(board, player, opponent, start.y, start.x);

		cell_pos = start;

		start.y += y_mover;
		start.x += x_mover;

		while(board->grid[start.y][start.x].token != NULL)
		{
			if(player_is_in(board->grid[start.y][start.x].token, strangleholds, 2))
				add_player_unique(board->grid[start.y][start.x].token,
								  board->grid[cell_pos.y][cell_pos.x].align_check_against, 2);

			check_borders_around(board, player, opponent, start.y, start.x);
			start.y += y_mover;
			start.x += x_mover;
			
			if(start.y == end.y && start.x == end.x)
				return;
		}

		start = cell_pos;
	}
}

void check_border(struct board* board, struct player* player, struct player* opponent,
				  uint8_t y, uint8_t x)
{
	printf("In check_border.\n");

	uint8_t player_token_around = 0;
	uint8_t opponent_token_around = 0;

	if(cell_is_in(board, y - 1, x))
	{
		if(board->grid[y - 1][x].token == player)
			player_token_around++;
		else if(board->grid[y - 1][x].token == opponent)
			player_token_around++;
	}

	if(cell_is_in(board, y - 1, x + 1))
	{
		if(board->grid[y - 1][x + 1].token == player)
			player_token_around++;
		else if(board->grid[y - 1][x + 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y, x + 1))
	{
		if(board->grid[y][x + 1].token == player)
			player_token_around++;
		else if(board->grid[y][x + 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y + 1, x + 1))
	{
		if(board->grid[y + 1][x + 1].token == player)
			player_token_around++;
		else if(board->grid[y + 1][x + 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y + 1, x))
	{
		if(board->grid[y + 1][x].token == player)
			player_token_around++;
		else if(board->grid[y + 1][x].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y + 1, x - 1))
	{
		if(board->grid[y + 1][x - 1].token == player)
			player_token_around++;
		else if(board->grid[y + 1][x - 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y, x - 1))
	{
		if(board->grid[y][x - 1].token == player)
			player_token_around++;
		else if(board->grid[y][x - 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y - 1, x - 1))
	{
		if(board->grid[y - 1][x - 1].token == player)
			player_token_around++;
		else if(board->grid[y - 1][x - 1].token == opponent)
			opponent_token_around++;
	}

	if(player_token_around >= 3)
		add_player_unique(opponent, board->grid[y][x].border_check_against, 2);
	if(opponent_token_around >= 3)
		add_player_unique(player, board->grid[y][x].border_check_against, 2);
}

void check_borders_around(struct board* board, struct player* player, struct player* opponent,
						  uint8_t y, uint8_t x)
{
	printf("In check_border_around.\n");

	if(cell_is_in(board, y - 1, x) && board->grid[y - 1][x].is_border &&
	   board->grid[y - 1][x].token == NULL) 
		check_border(board, player, opponent, y - 1, x);

	if(cell_is_in(board, y - 1, x + 1) && board->grid[y - 1][x + 1].is_border &&
	   board->grid[y - 1][x + 1].token == NULL) 
		check_border(board, player, opponent, y - 1, x + 1);

	if(cell_is_in(board, y, x + 1) && board->grid[y][x + 1].is_border &&
	   board->grid[y][x + 1].token == NULL) 
		check_border(board, player, opponent, y, x + 1);

	if(cell_is_in(board, y + 1, x + 1) && board->grid[y + 1][x + 1].is_border &&
	   board->grid[y + 1][x + 1].token == NULL) 
		check_border(board, player, opponent, y + 1, x + 1);

	if(cell_is_in(board, y + 1, x) && board->grid[y + 1][x].is_border &&
	   board->grid[y + 1][x].token == NULL) 
		check_border(board, player, opponent, y + 1, x);

	if(cell_is_in(board, y + 1, x - 1) && board->grid[y + 1][x - 1].is_border &&
	   board->grid[y + 1][x - 1].token == NULL) 
		check_border(board, player, opponent, y + 1, x - 1);

	if(cell_is_in(board, y, x - 1) && board->grid[y][x - 1].is_border &&
	   board->grid[y][x - 1].token == NULL) 
		check_border(board, player, opponent, y, x - 1);

	if(cell_is_in(board, y - 1, x - 1) && board->grid[y - 1][x - 1].is_border &&
	   board->grid[y - 1][x - 1].token == NULL) 
		check_border(board, player, opponent, y - 1, x - 1);
}

void play_move(struct board* board, struct player* player, struct player* opponent,
			   uint8_t y, uint8_t x)
{
	struct coord left = {.y = y, .x = 0};
	struct coord right = {.y = y, .x = board->length};
	struct coord top = {.y = 0, .x = x};
	struct coord bottom = {.y  = board->height, .x = x};

	uint8_t min = x < y ? x : y;
	uint8_t sum = x + y;

	struct coord top_left = {.y = y - min, .x = x - min};
	struct coord bottom_right = {.y = board->height - top_left.x, .x = board->length - top_left.y};
	struct coord top_right = {.y = sum > board->height ? sum - board->height : 0,
							  .x = sum > board->length ? board->length : sum};
	struct coord bottom_left = {.y = top_right.x, .x = top_right.y};
	
	printf("After coords init in play_move\n");

	printf("left = %d, %d\n", left.y, left.x);
	printf("right = %d, %d\n", right.y, right.x);
	printf("top = %d, %d\n", top.y, top.x);
	printf("bottom = %d, %d\n", bottom.y, bottom.x);
	printf("top_left = %d, %d\n", top_left.y, top_left.x);
	printf("bottom_right = %d, %d\n", bottom_right.y, bottom_right.x);
	printf("top_right = %d, %d\n", top_right.y, top_right.x);
	printf("bottom_left = %d, %d\n", bottom_left.y, bottom_left.x);

	board->grid[y][x].token = player;

	printf("Token played = %s\n", board->grid[y][x].token->name);

	eliminate_from_to(board->grid, player, opponent, left, right);
	eliminate_from_to(board->grid, player, opponent, top, bottom);
	eliminate_from_to(board->grid, player, opponent, top_left, bottom_right);
	eliminate_from_to(board->grid, player, opponent, top_right, bottom_left);

	eliminate_borders_around(board, player, opponent, y, x);

	check_from_to(board, player, opponent,left, right);
	check_from_to(board, player, opponent,top, bottom);
	check_from_to(board, player, opponent,top_left, bottom_right);
	check_from_to(board, player, opponent,top_right, bottom_left);
}

struct player* get_winner(struct game* game)
{
	uint8_t i;
	for(i = 0; i < 2; i++)
	{
		if(game->players[i]->points >= game->points_to_win)
			return game->players[i];
	}
	// Check if the board is playable
		// if the board board is not playable
		// The player with the higher points wins, otherwise the player with the less check agains him win.

	return NULL;
}
