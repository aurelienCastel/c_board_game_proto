#include<stdlib.h>

#include "player.h"
#include "util.h"
#include "token_types.h"

void init_player(struct player* player, char* name, uint8_t is_ai,
				 uint8_t team, color color, struct hero* hero)
{
	player->name = string_copy(name, string_length(name)); // malloc
	player->is_ai = is_ai;
	player->team = team;
	player->color = color;
	player->hero = hero;
	init_token(&(player->token), player, &Standard_type);
	init_token(&(player->special_token), player, hero->token_type);
	player->is_special_skill_available = 0;
	player->points = 0;
}

void delete_player(struct player player)
{
	free(player.name);
}
