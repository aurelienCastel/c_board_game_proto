#ifndef player_h
#define player_h

#include <stdint.h>

#include "hero.h"
#include "token.h"
#include "console_color.h"

struct player
{
	char* name;
	uint8_t is_ai;
	uint8_t number;
	uint8_t team;
	color color;
	struct hero* hero;
	struct token token;
	struct token special_token;
	uint8_t special_token_amount;
	uint8_t is_special_skill_available;
	uint8_t points;
};

void init_player(struct player* player, char* name, uint8_t is_ai,
				 uint8_t team, color color, struct hero* hero);

void delete_player(struct player player);

#endif
