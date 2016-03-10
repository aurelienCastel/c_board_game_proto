#include <stdlib.h>
#include <stddef.h>

#include "player.h"
#include "util.h"
#include "token_types.h"

// Look for error handling possible when allocating memory, could it be usefull to return something?
void init_player(struct player* a_player, char* name, uint8_t is_ai, uint8_t number, uint8_t team, char* color,
				struct hero* hero_played)
{
	a_player->name = string_copy(name, string_length(name)); // malloc
	a_player->is_ai = is_ai;
	a_player->number = number;
	a_player->team = team;
	a_player->color = string_copy(color, string_length(color)); // malloc
	a_player->hero = hero_played;
	init_token(&(a_player->token), a_player, &Standard_type);
	init_token(&(a_player->special_token), a_player, hero_played->token_type);
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
void delete_player(struct player a_player)
{
	free(a_player.name);
	free(a_player.color);
}
