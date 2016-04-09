#ifndef player_h
#define player_h

#include <stdint.h>

#include "console_color.h"

struct player
{
	char* name;
	uint8_t is_ai;
	color color;
	uint8_t points;
};

void init_player(struct player* player, char* name, uint8_t is_ai, color color);
void delete_player(struct player player);

uint8_t player_is_in(struct player* player, struct player players[], uint8_t length);
void add_player_unique(struct player* player, struct player players[], uint8_t length);
void add_player(struct player* player, struct player players[], uint8_t length);
void remove_player(struct player* player, struct player players[], uint8_t length);

#endif
