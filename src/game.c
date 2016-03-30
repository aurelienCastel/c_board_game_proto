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
		player_plays(game, game->players + i);
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

struct coord* string_to_coord(struct game* game, char* string)
{
	return NULL;
}

void player_plays(struct game* game, struct player* player)
{
	char* input;
	struct coord* coord;

	printf("It's %s turn to play.\n", player->name);
	if(!player->is_ai)
	{
		while(1)
		{
			printf("Enter your move:");
			coord = get_input(10);
			coord = string_to_coord(game, input);
			if(coord == NULL)
				bad_input_message();
			else
				break;
		}
	}
	else
	{
		printf("%s is thinking...\n", player->name);
	}
	play_move(game, player, coord);
}

void play_move(struct game* game, struct player* player, struct coord* coord)
{
	game->board->grid[coord.y][coord.x].token = &(game->players[player_nb].token);

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
