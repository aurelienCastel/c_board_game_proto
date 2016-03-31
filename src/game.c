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

uint8_t player_is_in_array(struct player* player, struct player check_against[])
{
	uint8_t i;

	for(i = 0; check_against + i != NULL; i++)
	{
		if(player == check_against + i)
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
	   player_is_in_array(player, board->grid[y][x].check_against))
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
	play_move(board, player, &coord);
}

void play_move(struct board* board, struct player* player, struct coord* coord)
{
	board->grid[coord->y][coord->x].token = &(player->token);

	//Update others cells according to this play (it have to be recursive, taking a token can change a check_against..)
}

struct player* get_winner(struct game* game)
{
	// Check points
	// Check if the board is playable
		// if the board board is not playable
		// The player with the higher points wins, otherwise the player with the less check agains him win.

	return NULL;
}
