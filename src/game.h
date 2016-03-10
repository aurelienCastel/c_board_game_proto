#ifndef game_h
#define game_h

#include <stdint.h>

struct player;
struct board;

// Maybe make a struct player_pool, that contains all players and nb_player.
struct game
{
	struct player* players;
	struct board* board;
	uint8_t player_nb;
	int turn_time;
	uint8_t points_to_win;
};

#endif
