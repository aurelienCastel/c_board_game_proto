#include<stdlib.h>

#include "player.h"
#include "util.h"

void init_player(struct player* player, struct player* opponent,
				 char* name, uint8_t is_ai, color color)
{
	player->name = string_copy(name, string_length(name)); // malloc
	player->opponent = opponent;
	player->is_ai = is_ai;
	player->color = color;
	player->points = 0;
}

void delete_player(struct player player)
{
	free(player.name);
}
