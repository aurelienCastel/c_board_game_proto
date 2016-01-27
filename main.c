#include <stdio.h>
#include <stdlib.h>
#include "general_debug.h"

#define ERROR_HANDLING

// I don't know if this struct will be usefull later.
struct token_type
{
	char* name;
};
typedef struct token_type token_type;

token_type new_token_type(char* name);
token_type new_token_type(char* name)
{
	token_type a_token_type;
	a_token_type.name = malloc((string_length(name) + 1) * sizeof(*name));
	a_token_type.name = string_copy(name);

	return a_token_type;
}

int delete_token_type(token_type a_token_type);
int delete_token_type(token_type a_token_type)
{
	// Test HERE if a_token.name is NULL.
	// Test HERE if a_token.name have not been allocated with malloc.  

	free(a_token_type.name);
	return 0;
	
	#ifdef ERROR_HANDLING
		error:
			return 1;
	#endif
}

struct token
{
	struct token_type* type;
	struct player* owner;
};
typedef struct token token;

token new_token(struct token_type* type, struct player* owner);
token new_token(struct token_type* type, struct player* owner)
{
	token a_token;
	a_token.type = type;
	a_token.owner = owner;
	return a_token;
}
// Useless to delete this struct, there is no malloced properties that we want to free.

struct hero
{
	// ? special_skill
	struct token_type token;
	// char token_nb_start; // Maybe some hero start with more or less than 1 special token.
};
typedef struct hero hero;

// Maybe we will add special_skill and token_nb_start to this constructor.
hero new_hero(struct token_type* type);
hero new_hero(struct token_type* type)
{
	hero a_hero;
	a_hero.token = type;
	return a_hero;
}
// Useless to delete this struct, there is no malloced properties that we want to free.
// If we had special_skill to it we will maybe need a destructor.

struct player
{
	char is_ai;
	char number;
	char team;
	char color; // This should be a pointer to a real color like rgb.
	struct hero* hero_played;
	struct token standard_token;
	struct token special_token;
	char special_token_amount;
	char special_skill_aviable;
	char points;
};
typedef struct player player;

player new_player(char is_ai, char player_nb, char team, char color,
					struct hero* hero_played);

player new_player(char is_ai, char player_nb, char team, char color,
					struct hero* hero_played)
{
	player a_player;
	a_player.is_ai = is_ai;
	a_player.player_nb = player_nb;
	a_player.team = team;
	a_player.color = color;
	a_player.hero_played = hero_played;
	a_player.standard_token = new_token(&(hero_played->token), struct player* owner);
	a_player.special_token = new_token();
	return a_player;
}

struct coordinate
{
	unsigned char x;
	unsigned char y;
};
typedef struct coordinate coordinate;

struct cell
{
	char is_hole;
	char is_border;
	struct token* p_token;
};
typedef struct cell cell;

struct game
{
	struct player* players;
	struct cell** board;
	int turn_time;
	char point_to_win;
};
typedef struct game game;

void main()
{
}
