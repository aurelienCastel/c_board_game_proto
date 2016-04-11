#include<stdlib.h>

#include "player.h"
#include "util.h"

void init_player(struct player* player, struct player* opponent, char* name, uint8_t is_ai, color color)
{
	player->name = string_copy(name, string_length(name)); // malloc
	player->is_ai = is_ai;
	player->color = color;
	player->points = 0;
	player->opponent = opponent;
}

void delete_player(struct player player)
{
	free(player.name);
}

uint8_t player_is_in(struct player* player, struct player* players[2], uint8_t length)
{
	uint8_t i;

	for(i = 0; i < length; i++)
	{
		if(players[i] == player)
			return 1;
	}

	return 0;
}

void add_player_unique(struct player* player, struct player* players[2], uint8_t length)
{
	uint8_t i;

	for(i = 0; i < length; i++)
	{
		if(players[i] == player)
			return;
		if(players[i] == NULL)
		{
			players[i] = player;
			return;
		}
	}
}

void add_player(struct player* player, struct player* players[2], uint8_t length)
{	
	uint8_t i;

	for(i = 0; i < length; i++)
	{
		if(players[i] == NULL)
		{
			players[i] = player;
			return;
		}
	}
	
}

void remove_player(struct player* player, struct player* players[2], uint8_t length)
{
	uint8_t i, pos;

	for(i = 0; i < length; i++)
	{
		if(players[i] == player)
			break;
	}

	for(pos = i; pos < length - 1; pos++)
	{
		players[pos] = players[pos + 1];
	}

	players[pos] = NULL;
}
