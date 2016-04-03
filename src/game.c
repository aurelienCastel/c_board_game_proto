#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "game.h"
#include "util.h"

void init_game(struct game* game, struct player players[],
			   struct board* board, uint8_t nb_players,
			   uint8_t points_to_win,  uint16_t turn_time)
{
	game->players = players;
	game->board = board;
	game->nb_players = nb_players;
	game->points_to_win = points_to_win;
	game->turn_time = turn_time;
}

void launch_game(struct game* game)
{
	uint8_t i;
	struct player* winner;

	determine_order(game->players, game->nb_players);
	sort_by_order(game->players, 0, game->nb_players - 1);
	printf("Here is the order in wihch players gonna plays:\n");
	print_players(game->players, game->nb_players);

	print_board(game->board);

	for(i = 0; i < game->nb_players; i++)
	{
		player_plays(game->board, game->players + i);
		winner = get_winner(game);
		if(winner != NULL)
			break;
		if(i == game->nb_players - 1)
			i = 0;
	}

	printf("The winner of the game is %s.\n", winner->name);
}

void determine_order(struct player players[], uint8_t nb_players)
{
	srand(time(NULL));

	uint8_t i;

	for(i = 0; i < nb_players; i++)
	{
		players[i].order = rand() % nb_players;
		if(is_order_taken(players, i))
			i--;
	}
}

uint8_t is_order_taken(struct player players[], uint8_t player_nb)
{
	uint8_t i;
	for(i = 0; i < player_nb; i++)
	{
		if(players[player_nb].order == players[i].order)
			return 1;
	}
	return 0;
}

void swap(struct player players[], uint8_t index1, uint8_t index2)
{
	struct player temp = players[index1];

	players[index1] = players[index2];

	players[index2] = temp;
}

void sort_by_order(struct player players[], uint8_t start, uint8_t end)
{
	uint8_t left = start - 1;
	uint8_t right = end + 1;
	struct player pivot = players[start];

	if(start >= end)
		return;

	while(1)
	{
		do right--; while(players[right].order > pivot.order);
		do left++; while(players[left].order < pivot.order);

		if(left < right)
			swap(players, left, right);

		else
			break;
	}

	sort_by_order(players, start, right);
	sort_by_order(players, right + 1, end);
}

void print_players(struct player players[], uint8_t nb_players)
{
	uint8_t i;

	for(i = 0; i < nb_players; i++)
	{
		printf("%s", players[i].name);

		if(i == nb_players - 1)
			printf("\n");
		else
			printf(" |");
	}
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

	for(i = 0; players[i] != NULL; i++)
	{
		if(player == players[i])
			return 1;
	}

	return 0;
}

uint8_t is_cell_present(struct board* board, uint8_t y, uint8_t x)
{
	if(y < 0 || y >= board->nb_rows ||
	   x < board->row_info[y].limit_left || x > board->row_info[y].limit_right ||
	   board->grid[y][x].is_hole)
		return 0;
	return 1;
}

uint8_t is_cell_playable_for(struct player* player, struct board* board, uint8_t y, uint8_t x)
{ 
	if(!is_cell_present(board, y, x) || board->grid[y][x].token != NULL ||
	   player_is_in_array(player, board->grid[y][x].check_for))
		return 0;
	return 1;
}

void player_plays(struct board* board, struct player* player)
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
			if(!is_cell_playable_for(player, board, coord.y, coord.x)) // Add a different message when the coord is not playable.
				bad_input_message();
			else
				break;
		}
	}
	else
	{
		printf("%s is thinking...\n", player->name);
	}
	play_move(board, player, coord.y, coord.x);
}

void eliminate_from_to(struct cell** grid, struct player* player, struct coord start, struct coord end)
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

		while(grid[start.y][start.x].token->owner != player)
		{
			if(start.y == end.y && start.x == end.x)
				return;

			start.y += y_mover;
			start.x += x_mover;
		}

		while(grid[start.y][start.x].token->owner == player)
		{
			if(start.y == end.y && start.x == end.x)
				return;

				start.y += y_mover;
				start.x += x_mover;
		}

		if(grid[start.y][start.x].token != NULL)
			chain_pos = start;
		else
			continue;

		while(grid[start.y][start.x].token != NULL &&
			  grid[start.y][start.x].token->owner != player)
		{
			if(start.y == end.y && start.x == end.x)
				return;

			start.y += y_mover;
			start.x += x_mover;
		}

		if(grid[start.y][start.x].token->owner == player)
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

void eliminate_border(struct board* board, struct player* player, uint8_t y, uint8_t x)
{
	uint8_t ennemi_token_around = 0;

	if(is_cell_present(board, y - 1, x) && board->grid[y - 1][x].token->owner == player)
		ennemi_token_around++;

	if(is_cell_present(board, y - 1, x + 1) && board->grid[y - 1][x + 1].token->owner == player)
		ennemi_token_around++;

	if(is_cell_present(board, y, x + 1) && board->grid[y][x + 1].token->owner == player)
		ennemi_token_around++;

	if(is_cell_present(board, y + 1, x + 1) && board->grid[y + 1][x + 1].token->owner == player)
		ennemi_token_around++;

	if(is_cell_present(board, y + 1, x) && board->grid[y + 1][x].token->owner == player)
		ennemi_token_around++;

	if(is_cell_present(board, y + 1, x - 1) && board->grid[y + 1][x - 1].token->owner == player)
		ennemi_token_around++;

	if(is_cell_present(board, y, x - 1) && board->grid[y][x - 1].token->owner == player)
		ennemi_token_around++;

	if(is_cell_present(board, y - 1, x - 1) && board->grid[y - 1][x - 1].token->owner == player)
		ennemi_token_around++;

	if(ennemi_token_around >= 3)
	{
		board->grid[y][x].token = NULL;
		player->points++;
	}
}

void eliminate_borders_around(struct board* board, struct player* player, uint8_t y, uint8_t x)
{
	if(is_cell_present(board, y - 1, x) && board->grid[y - 1][x].is_border &&
	   board->grid[y - 1][x].token != NULL && board->grid[y - 1][x].token->owner != player) 
		eliminate_border(board, player, y - 1, x);

	if(is_cell_present(board, y - 1, x + 1) && board->grid[y - 1][x + 1].is_border &&
	   board->grid[y - 1][x + 1].token != NULL && board->grid[y - 1][x + 1].token->owner != player) 
		eliminate_border(board, player, y - 1, x + 1);

	if(is_cell_present(board, y, x + 1) && board->grid[y][x + 1].is_border &&
	   board->grid[y][x + 1].token != NULL && board->grid[y][x + 1].token->owner != player) 
		eliminate_border(board, player, y, x + 1);

	if(is_cell_present(board, y + 1, x + 1) && board->grid[y + 1][x + 1].is_border &&
	   board->grid[y + 1][x + 1].token != NULL && board->grid[y + 1][x + 1].token->owner != player) 
		eliminate_border(board, player, y + 1, x + 1);

	if(is_cell_present(board, y + 1, x) && board->grid[y + 1][x].is_border &&
	   board->grid[y + 1][x].token != NULL && board->grid[y + 1][x].token->owner != player) 
		eliminate_border(board, player, y + 1, x);

	if(is_cell_present(board, y + 1, x - 1) && board->grid[y + 1][x - 1].is_border &&
	   board->grid[y + 1][x - 1].token != NULL && board->grid[y + 1][x - 1].token->owner != player) 
		eliminate_border(board, player, y + 1, x - 1);

	if(is_cell_present(board, y, x - 1) && board->grid[y][x - 1].is_border &&
	   board->grid[y][x - 1].token != NULL && board->grid[y][x - 1].token->owner != player) 
		eliminate_border(board, player, y, x - 1);

	if(is_cell_present(board, y - 1, x - 1) && board->grid[y - 1][x - 1].is_border &&
	   board->grid[y - 1][x - 1].token != NULL && board->grid[y - 1][x - 1].token->owner != player) 
		eliminate_border(board, player, y - 1, x - 1);
}

void check_from_to(struct cell** grid, struct player* player, struct coord start, struct coord end)
{
	uint8_t i;
	int8_t y_mover = 0;
	int8_t x_mover = 0;
	uint8_t nb_strangleholds;
	struct player* strangleholds[3];
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
		nb_strangleholds = 0;
		strangleholds[nb_strangleholds] = NULL;

		while(grid[start.y][start.x].token == NULL)
		{
			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		while(grid[start.y][start.x].token != NULL)
		{
			if(!player_is_in_array(grid[start.y][start.x].token->owner, strangleholds))
			{
				strangleholds[nb_strangleholds] = grid[start.y][start.x].token->owner;
				nb_strangleholds++;
				strangleholds[nb_strangleholds] = NULL;
			}

			start.y += y_mover;
			start.x += x_mover;

			if(start.y == end.y && start.x == end.x)
				return;
		}

		cell_pos = start;

		start.y += y_mover;
		start.x += x_mover;

		for(i = 0; grid[start.y][start.x].token != NULL;)
		{
			if(player_is_in_array(grid[start.y][start.x].token->owner, strangleholds))
			{
				if(!player_is_in_array(grid[start.y][start.x].token->owner, grid[cell_pos.y][cell_pos.x].check_for))
				{
					grid[cell_pos.y][cell_pos.x].check_for[i] = grid[start.y][start.x].token->owner;
					i++;
					grid[cell_pos.y][cell_pos.x].check_for[i] = NULL;
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

void check_borders_from_to(struct cell** grid, struct player* player, struct coord start, struct coord end)
{
}

void play_move(struct board* board, struct player* player, uint8_t y, uint8_t x)
{
	struct coord left = {.y = y, .x = board->row_info[y].limit_left};
	struct coord right = {.y = y, .x = board->row_info[y].limit_right + 1};
	struct coord top = {.y = 0, .x = x};
	struct coord bottom = {.y  = board->nb_rows, .x = x};
	struct coord top_left = {.y = 0, .x = board->row_info[0].limit_left};
	struct coord bottom_right = {.y = board->nb_rows, .x = board->row_info[board->nb_rows - 1].limit_right + 1};
	struct coord top_right = {.y = 0, .x = board->row_info[0].limit_right};
	struct coord bottom_left = {.y = board->nb_rows, .x = board->row_info[board->nb_rows - 1].limit_left - 1};

	board->grid[y][x].token = &(player->token);

	eliminate_from_to(board->grid, player, left, right);
	eliminate_from_to(board->grid, player, top, bottom);
	eliminate_from_to(board->grid, player, top_left, bottom_right);
	eliminate_from_to(board->grid, player, top_right, bottom_left);

	eliminate_borders_around(board, player, y, x);

	check_from_to(board->grid, player, left, right);
	check_from_to(board->grid, player, top, bottom);
	check_from_to(board->grid, player, top_left, bottom_right);
	check_from_to(board->grid, player, top_right, bottom_left);

	check_borders_from_to(board->grid, player, left, right);
	check_borders_from_to(board->grid, player, top, bottom);
	check_borders_from_to(board->grid, player, top_left, bottom_right);
	check_borders_from_to(board->grid, player, top_right, bottom_left);
}

struct player* get_winner(struct game* game)
{
	// Check points
	// Check if the board is playable
		// if the board board is not playable
		// The player with the higher points wins, otherwise the player with the less check agains him win.

	return NULL;
}
