#ifndef game_h
#define game_h

#include <stdint.h>

struct player;
struct board;

struct game
{
	struct player* players;
	struct board* board;
	uint8_t player_nb;
	uint8_t points_to_win;
	uint16_t turn_time;
};

void init_game(struct game* game, struct player players[],
			   struct board* board, uint8_t player_nb,
			   uint8_t points_to_win, uint16_t turn_time);

void launch_game(struct game* game);

#endif
