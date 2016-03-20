#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#include "game.h"

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
	determine_order(game->players, game->nb_players);
	sort_by_order(game->players);
	printf("Here is the order in wihch players gonna plays:\n");

	// Print board
	// player plays
		// Check if the cell exist
		// Check if the cell is playable
		// Update the cell
		// Update others cells according to this play (it have to be recursive, taking a token can change a check_against..)
		// Check points
		// Check if the board is playable
			// if the board board is not playable
			// The player with the higher points wins, otherwise the player with the less check agains him win.
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

void sort_by_order(struct player players[])
{
	void swap(struct player players[],
			  uint8_t index1, uint8_t index2)
	{
		struct player temp = players[index1];

		players[index1] = players[index2];

		players[index2] = temp;
	}

	void quickSort(int tableau[], int debut, int fin)
	{
	int gauche = debut-1;

	int droite = fin+1;

	const int pivot = tableau[debut];


	/* Si le tableau est de longueur nulle, il n'y a rien à faire. */

	if(debut >= fin)

		return;


	/* Sinon, on parcourt le tableau, une fois de droite à gauche, et une

	   autre de gauche à droite, à la recherche d'éléments mal placés,

	   que l'on permute. Si les deux parcours se croisent, on arrête. */

	while(1)

	{

		do droite--; while(tableau[droite] > pivot);

		do gauche++; while(tableau[gauche] < pivot);


		if(gauche < droite)

			echanger(tableau, gauche, droite);

		else break;

	}


	/* Maintenant, tous les éléments inférieurs au pivot sont avant ceux

	   supérieurs au pivot. On a donc deux groupes de cases à trier. On utilise

	   pour cela... la méthode quickSort elle-même ! */

	quickSort(tableau, debut, droite);

	quickSort(tableau, droite+1, fin);

	}
}
