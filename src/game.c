#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "game.h"
#include "util.h"

void init_game(struct game* game, struct player* players,
			   struct board* board, uint8_t points_to_win, uint16_t turn_time)
{
	game->players = players;
	game->board = board;
	game->points_to_win = points_to_win;
	game->turn_time = turn_time;
}

void launch_game(struct game* game)
{
	srand(time(NULL));

	struct player* current_player = game->players;
	struct coord coord;

	if(rand() % 2)
		current_player = current_player->opponent;

	printf("%s play first.\n", current_player->name);

	do
	{
		print_board(game->board);
		coord = move_request(game->board, current_player);

		printf("After coord request: %d, %d\n", coord.y, coord.x);

		play_move(game->board, current_player, current_player->opponent, coord.y, coord.x);
		current_player = current_player->opponent;
	}while(get_winner(game) == NULL);

	printf("The winner of the game is %s.\n", current_player->opponent->name);
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

uint8_t player_is_in_array(struct player* player, struct player* players[])
{
	uint8_t i;

	for(i = 0; i < 2; i++)
	{
		if(player == players[i])
			return 1;
	}

	return 0;
}

uint8_t cell_is_in(struct board* board, uint8_t y, uint8_t x)
{
	return y < board->height && x < board->length;
}

uint8_t cell_is_playable_for(struct player* player, struct board* board, uint8_t y, uint8_t x)
{ 
	if(!cell_is_in(board, y, x) ||
	   board->grid[y][x].is_hole|| board->grid[y][x].token != NULL ||
	   player_is_in_array(player, board->grid[y][x].check_against))
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

	if(start.x > end.x)
		y_mover = -1;
	else if(start.x < end.x)
		y_mover = 1;
	if(start.y > end.y)
		x_mover = -1;
	else if(start.y < end.y)
		x_mover = 1;

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

void check_from_to(struct cell** grid, struct coord start, struct coord end)
{
	uint8_t i;
	int8_t y_mover = 0;
	int8_t x_mover = 0;
	struct player* strangleholds[2];
	struct coord cell_pos;

	if(start.x > end.x)
		y_mover = -1;
	else if(start.x < end.x)
		y_mover = 1;
	if(start.y > end.y)
		x_mover = -1;
	else if(start.y < end.y)
		x_mover = 1;

	while(start.y != end.y || start.x != end.x)
	{
		strangleholds[0] = NULL;
		strangleholds[1] = NULL;

		while(grid[start.y][start.x].token == NULL)
		{
			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		for(i = 0; grid[start.y][start.x].token != NULL;)
		{
			if(!player_is_in_array(grid[start.y][start.x].token, strangleholds))
			{
				strangleholds[i] = grid[start.y][start.x].token;
				i++;
			}

			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		cell_pos = start;
		grid[cell_pos.y][cell_pos.x].check_against[0] = NULL;
		grid[cell_pos.y][cell_pos.x].check_against[1] = NULL;

		start.y += y_mover;
		start.x += x_mover;

		for(i = 0; grid[start.y][start.x].token != NULL;)
		{
			if(player_is_in_array(grid[start.y][start.x].token, strangleholds))
			{
				if(!player_is_in_array(grid[start.y][start.x].token, grid[cell_pos.y][cell_pos.x].check_against))
				{
					grid[cell_pos.y][cell_pos.x].check_against[i] = grid[start.y][start.x].token->opponent;
					i++;
				}
			}

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
	uint8_t player_token_around = 0;
	uint8_t opponent_token_around = 0;

	board->grid[y][x].check_against[0] = NULL;
	board->grid[y][x].check_against[1] = NULL;

	if(cell_is_in(board, y - 1, x))
	{
		if(board->grid[y - 1][x].token == player)
			player_token_around++;
		if(board->grid[y - 1][x].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y - 1, x + 1))
	{
		if(board->grid[y - 1][x + 1].token == player)
			player_token_around++;
		if(board->grid[y - 1][x + 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y, x + 1))
	{
		if(board->grid[y][x + 1].token == player)
			player_token_around++;
		if(board->grid[y][x + 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y + 1, x + 1))
	{
		if(board->grid[y + 1][x + 1].token == player)
			player_token_around++;
		if(board->grid[y + 1][x + 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y + 1, x))
	{
		if(board->grid[y + 1][x].token == player)
			player_token_around++;
		if(board->grid[y + 1][x].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y + 1, x - 1))
	{
		if(board->grid[y + 1][x - 1].token == player)
			player_token_around++;
		if(board->grid[y + 1][x - 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y, x - 1))
	{
		if(board->grid[y][x - 1].token == player)
			player_token_around++;
		if(board->grid[y][x - 1].token == opponent)
			opponent_token_around++;
	}

	if(cell_is_in(board, y - 1, x - 1))
	{
		if(board->grid[y - 1][x - 1].token == player)
			player_token_around++;
		if(board->grid[y - 1][x - 1].token == opponent)
			opponent_token_around++;
	}

	if(player_token_around >= 3)
		board->grid[y][x].check_against[0] = player;

	if(opponent_token_around >= 3)
		board->grid[y][x].check_against[1] = opponent;
}

void check_borders_around(struct board* board, struct player* player, struct player* opponent,
							  uint8_t y, uint8_t x)
{
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

void check_borders_from_to(struct board* board, struct player* player, struct player* opponent,
						   struct coord start, struct coord end)
{
	int8_t y_mover = 0;
	int8_t x_mover = 0;

	if(start.x > end.x)
		y_mover = -1;
	else if(start.x < end.x)
		y_mover = 1;
	if(start.y > end.y)
		x_mover = -1;
	else if(start.y < end.y)
		x_mover = 1;

	while(start.y != end.y || start.x != end.x)
	{
		if(board->grid[start.y][start.x].is_border && board->grid[start.y][start.x].token == NULL)
			check_border(board, player, opponent, start.y, start.x);
		check_borders_around(board, player, opponent, start.y, start.x);

		start.y += y_mover;
		start.x += x_mover;
	}
}

void play_move(struct board* board, struct player* player, struct player* opponent,
			   uint8_t y, uint8_t x)
{
	struct coord left = {.y = y, .x = board->length - 1};
	struct coord right = {.y = y, .x = 0};
	struct coord top = {.y = 0, .x = x};
	struct coord bottom = {.y  = board->height - 1, .x = x};

	struct coord top_left = {.y = 0, .x = board->row_info[0].limit_left};
	struct coord bottom_right = {.y = board->nb_rows - 1, .x = board->row_info[board->nb_rows - 1].limit_right};
	struct coord top_right = {.y = 0, .x = board->row_info[0].limit_right};
	struct coord bottom_left = {.y = board->nb_rows - 1, .x = board->row_info[board->nb_rows - 1].limit_left};
	
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

	eliminate_from_to(board->grid, player, opponent, left, right);
	eliminate_from_to(board->grid, player, opponent, top, bottom);
	eliminate_from_to(board->grid, player, opponent, top_left, bottom_right);
	eliminate_from_to(board->grid, player, opponent, top_right, bottom_left);

	eliminate_borders_around(board, player, opponent, y, x);

	check_from_to(board->grid, left, right);
	check_from_to(board->grid, top, bottom);
	check_from_to(board->grid, top_left, bottom_right);
	check_from_to(board->grid, top_right, bottom_left);

	check_borders_from_to(board, player, opponent, left, right);
	check_borders_from_to(board, player, opponent, top, bottom);
	check_borders_from_to(board, player, opponent, top_left, bottom_right);
	check_borders_from_to(board, player, opponent, top_right, bottom_left);
}

struct player* get_winner(struct game* game)
{
	uint8_t i;

	for(i = 0; i < 2; i++)
	{
		if(game->players[i].points >= game->points_to_win)
			return &game->players[i];
	}
	// Check if the board is playable
		// if the board board is not playable
		// The player with the higher points wins, otherwise the player with the less check agains him win.

	return NULL;
}
