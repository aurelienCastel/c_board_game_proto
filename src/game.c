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

	struct player* current_player = game->players[0];
	struct coord coord;

	if(rand() % 2)
		current_player = current_player->opponent;

	printf("%s play first.\n", current_player->name);

	do
	{
		print_board(game->board);
		coord = move_request(game->board, current_player);
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

uint8_t cell_is_in(struct board* board, uint8_t y, uint8_t x)
{
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
		coord.y  = 0;
		coord.x  = 0;
		// coord = struct coord ai_decision(...)
	}

	return coord;
}

struct card_points get_card_limits(struct board* board, uint8_t y, uint8_t x)
{
	uint8_t min = x < y ? x : y;
	uint8_t sum = x + y;
	struct card_points card_limits =
	{
		.left = {.y = y, .x = 0},
		.right = {.y = y, .x = board->length},
		.top = {.y = 0, .x = x},
		.bottom = {.y  = board->height, .x = x},

		.top_left = {.y = y - min, .x = x - min},
		.bottom_right = {.y = board->height - card_limits.top_left.x,
						 .x = board->length - card_limits.top_left.y},
		.top_right = {.y = sum > board->height ? sum - board->height : 0,
					  .x = sum > board->length ? board->length : sum},
		.bottom_left = {.y = card_limits.top_right.x, .x = card_limits.top_right.y},
	};

//	printf("left: %d, %d\n", card_limits.left.y, card_limits.left.x);
//	printf("right: %d, %d\n", card_limits.right.y, card_limits.right.x);
//	printf("top: %d, %d\n", card_limits.top.y, card_limits.top.x);
//	printf("bottom: %d, %d\n", card_limits.bottom.y, card_limits.bottom.x);
//	printf("top_left: %d, %d\n", card_limits.top_left.y, card_limits.top_left.x);
//	printf("bottom_right: %d, %d\n", card_limits.bottom_right.y, card_limits.bottom_right.x);
//	printf("top_right: %d, %d\n", card_limits.top_right.y, card_limits.top_right.x);
//	printf("bottom_left: %d, %d\n", card_limits.bottom_left.y, card_limits.bottom_left.x);

	return card_limits;
}

void uncheck_from_to(struct board* board, struct coord start, struct coord end)
{
	printf("In uncheck_from_to\n");

	int8_t y_mover = 0;
	int8_t x_mover = 0;

	if(start.y < end.y)
		y_mover = 1;
	if(start.x > end.x)
		x_mover = -1;
	else if(start.x < end.x)
		x_mover = 1;

	while(start.y != end.y || start.x != end.x)
	{
		if(board->grid[start.y][start.x].token == NULL)
		{
			board->grid[start.y][start.x].align_check_against[0] = NULL;
			board->grid[start.y][start.x].align_check_against[1] = NULL;
		}
		printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
		start.y += y_mover;
		start.x += x_mover;
	}
}

void eliminate_from_to(struct board* board, struct player* player, struct player* opponent,
					   struct coord start, struct coord end)
{
	printf("In eliminate_from_to\n\t");

	int8_t y_mover = 0;
	int8_t x_mover = 0;
	struct coord chain_pos;
	struct card_points card_limits;

	if(start.y < end.y)
		y_mover = 1;
	if(start.x > end.x)
		x_mover = -1;
	else if(start.x < end.x)
		x_mover = 1;

	while(start.y != end.y || start.x != end.x)
	{
		chain_pos.y = 0;
		chain_pos.x = 0;

		while(board->grid[start.y][start.x].token != player)
		{
			if(start.y == end.y && start.x == end.x)
				return;

			printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
			start.y += y_mover;
			start.x += x_mover;
		}

		while(board->grid[start.y][start.x].is_hole || board->grid[start.y][start.x].token == player)
		{
			if(start.y == end.y && start.x == end.x)
				return;

				printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
				start.y += y_mover;
				start.x += x_mover;
		}

		if(board->grid[start.y][start.x].token == opponent)
			chain_pos = start;
		else
			continue;

		while(board->grid[start.y][start.x].is_hole ||
			  board->grid[start.y][start.x].token == opponent)
		{
			if(start.y == end.y && start.x == end.x)
				return;

			printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
			start.y += y_mover;
			start.x += x_mover;
		}

		if(board->grid[start.y][start.x].token == player)
		{
				while(chain_pos.y != start.y || chain_pos.x != start.x)
				{
					board->grid[chain_pos.y][chain_pos.x].token = NULL;
					player->points++;

					card_limits = get_card_limits(board, chain_pos.y, chain_pos.x);
					uncheck_from_to(board, card_limits.left, card_limits.right);
					uncheck_from_to(board, card_limits.top, card_limits.bottom);
					uncheck_from_to(board, card_limits.top_left, card_limits.bottom_right);
					uncheck_from_to(board, card_limits.top_right, card_limits.bottom_left);

					check_from_to(board, player, opponent, card_limits.left, card_limits.right);
					check_from_to(board, player, opponent, card_limits.top, card_limits.bottom);
					check_from_to(board, player, opponent, card_limits.top_left, card_limits.bottom_right);
					check_from_to(board, player, opponent, card_limits.top_right, card_limits.bottom_left);

					chain_pos.y += y_mover;
					chain_pos.x += x_mover;
				}
		}
	}
}

void eliminate_border(struct board* board, struct player* player,
					  uint8_t y, uint8_t x)
{
	printf("In eliminate_border\n");

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
	printf("In eliminate_border_around\n");

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

// Maybe write another function (check_borders_from_to) because mixing both makes it repeat
// uselessy check_border_around.
void check_from_to(struct board* board, struct player* player, struct player* opponent,
				   struct coord start, struct coord end)
{
	printf("In check_from_to\n\t");

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
		//	if(board->grid[start.y][start.x].is_border)
		//		check_border(board, player, opponent, start.y, start.x);
			check_borders_around(board, player, opponent, start.y, start.x);

			printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		while(board->grid[start.y][start.x].token != NULL)
		{
			add_player_unique(board->grid[start.y][start.x].token, strangleholds, 2);

			check_borders_around(board, player, opponent, start.y, start.x);

			printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		//if(board->grid[start.y][start.x].is_border)
		//	check_border(board, player, opponent, start.y, start.x);
		check_borders_around(board, player, opponent, start.y, start.x);

		cell_pos = start;

		printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
		start.y += y_mover;
		start.x += x_mover;

		while(board->grid[start.y][start.x].token != NULL)
		{
			if(player_is_in(board->grid[start.y][start.x].token, strangleholds, 2))
				add_player_unique(board->grid[start.y][start.x].token->opponent,
								  board->grid[cell_pos.y][cell_pos.x].align_check_against, 2);

			printf("y:%d | x:%d (end.y:%d | end.x:%d)\n", start.y, start.x, end.y, end.x);
			check_borders_around(board, player, opponent, start.y, start.x);

			if(start.y == end.y && start.x == end.x)
				return;
			start.y += y_mover;
			start.x += x_mover;
			
		}

		start = cell_pos;
	}
}

void check_border(struct board* board, struct player* player, struct player* opponent,
				  uint8_t y, uint8_t x)
{
	printf("\tIn check_border\n");

	uint8_t player_token_around = 0;
	uint8_t opponent_token_around = 0;

	board->grid[y][x].border_check_against[0] = NULL;
	board->grid[y][x].border_check_against[1] = NULL;

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
	printf("\tIn check_border_around\n");

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
	struct card_points card_limits = get_card_limits(board, y, x);
	
	board->grid[y][x].token = player;

	eliminate_from_to(board, player, opponent, card_limits.left, card_limits.right);
	eliminate_from_to(board, player, opponent, card_limits.top, card_limits.bottom);
	eliminate_from_to(board, player, opponent, card_limits.top_left, card_limits.bottom_right);
	eliminate_from_to(board, player, opponent, card_limits.top_right, card_limits.bottom_left);

	eliminate_borders_around(board, player, opponent, y, x);

	check_from_to(board, player, opponent, card_limits.left, card_limits.right);
	check_from_to(board, player, opponent, card_limits.top, card_limits.bottom);
	check_from_to(board, player, opponent, card_limits.top_left, card_limits.bottom_right);
	check_from_to(board, player, opponent, card_limits.top_right, card_limits.bottom_left);
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
