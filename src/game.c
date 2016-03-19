#include "game.h"

void init_game(struct game* game, struct player players[],
		  struct board* board, uint8_t player_nb,
		  uint8_t points_to_win,  uint16_t turn_time)
{
	game->players = players;
	game->board = board;
	game->player_nb = player_nb;
	game->points_to_win = points_to_win;
	game->turn_time = turn_time;
}

void launch_game(struct game* game)
{
	// Flip coin

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
