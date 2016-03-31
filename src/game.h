#ifndef game_h
#define game_h

#include <stdint.h>

#include "player.h"
#include "board.h"

struct game
{
	struct player* players;
	struct board* board;
	uint8_t nb_players;
	uint8_t points_to_win;
	uint16_t turn_time;
};

void init_game(struct game* game, struct player players[],
			   struct board* board, uint8_t nb_players,
			   uint8_t points_to_win, uint16_t turn_time);

void launch_game(struct game* game);

void determine_order(struct player players[], uint8_t nb_players);
uint8_t is_order_taken(struct player players[], uint8_t player_nb);
void sort_by_order(struct player players[], uint8_t start, uint8_t end);
void swap(struct player players[], uint8_t index1, uint8_t index2);
void print_players(struct player players[], uint8_t nb_players);

struct coord string_to_coord(char* string);
uint8_t player_is_in_array(struct player* player, struct player check_against[]);
uint8_t is_cell_present(struct board* board, uint8_t y, uint8_t x);
uint8_t is_cell_playable_for(struct player* player, struct board* board, uint8_t y, uint8_t x);

void player_plays(struct board* board, struct player* player);
void play_move(struct board* board, struct player* player, struct coord* coord);
struct player* get_winner(struct game* game);

#endif
