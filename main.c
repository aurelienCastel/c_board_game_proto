#include <stdio.h>
#include <stdlib.h>
#include "general_debug.h"

#define ERROR_HANDLING

char* string_copy(char* string);
char* string_copy(char* string)
{
}

// I don't know if this struct will be usefull later.
struct token_type
{
	char* name;
};
typedef struct token_type token_type;

// Look for error handling possible when allocating memory, could it be usefull to return something?
void new_token_type(struct token_type* a_token_type, char* name);
void new_token_type(struct token_type* a_token_type, char* name)
{
	a_token_type->name = string_copy(name); // malloc
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
void delete_token_type(token_type a_token_type);
void delete_token_type(token_type a_token_type)
{
	// Send a warning when freeing NULL pointers.
	free(a_token_type.name);
}

struct token
{
	struct token_type* type;
	struct player* owner;
};
typedef struct token token;

// I don't think this structure need to be stored on the heap, there is nothing to free in it.
// There it doesn't need to returns a pointer.

// There isn't even a need for a creator function, the struct can be initialized with {.a = ..., .c = ...}.
// So there is no need to delete it or something.

struct hero
{
	char* name;
	// ? special_skill
	struct token_type* token;
	// char token_nb_start; // Maybe some hero start with more or less than 1 special token.
};
typedef struct hero hero;

// Look for error handling possible when allocating memory, could it be usefull to return something?
// Maybe we will add special_skill and token_nb_start to this constructor.
// The hero own the token_type, once the hero is deleted, the token_type is deleted too.
void new_hero(struct hero* a_hero, char* name, struct token_type* type);
void new_hero(struct hero* a_hero, char* name, struct token_type* type)
{
	a_hero->token = type;
	a_hero->name = string_copy(name);
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
void delete_hero(struct hero a_hero);
void delete_hero(struct hero a_hero)
{
	free(a_hero.name);
	delete_token_type(*(a_hero.token));
}
// If we add special_skill to it we will maybe need a constructor and destructor.

struct player
{
	char* name;
	char is_ai;
	char number;
	char team;
	char color; // This should be a pointer to a real color like rgb.
	struct hero* hero;
	struct token token;
	struct token special_token;
	char special_token_amount;
	char special_skill_aviable;
	char points;
};
typedef struct player player;

// Look for error handling possible when allocating memory, could it be usefull to return something?
void new_player(struct player* a_player, char* name, char is_ai, char player_nb, char team, char color,
				struct hero* hero_played);

void new_player(struct player* a_player, char* name, char is_ai, char player_nb, char team, char color,
				struct hero* hero_played)
{
	a_player->name = string_copy(name); // malloc
	a_player->is_ai = is_ai;
	a_player->player_nb = player_nb;
	a_player->team = team;
	a_player->color = color;
	a_player->hero = hero_played;
	a_player->token = {.type = &Standard_type, .owner = a_player}; // Standard_type is a global.
	a_player->special_token = {.type = hero_played->token, .owner = a_player};
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
void delete_player(struct player a_player);
void delete_player(struct player a_player)
{
	free(a_player.name);
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
	struct token* token = NULL;
};
typedef struct cell cell;

// I don't think this structure need to be stored on the heap, there is nothing to free in it.
// There it doesn't need to returns a pointer.

// There isn't even a need for a creator function, the struct can be initialized with {.a = ..., .c = ...}.
// So there is no need to delete it or something.

struct row
{
	struct cell* cells;
	char length;
};
typedef struct row row;

// Look for error handling possible when allocating memory, could it be usefull to return something?
void new_row(struct row* a_row, char length)
{
	a_row->cells = malloc(length * sizeof *(a_row->cells));
	a_row->length = length;
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
void delete_row(struct row a_row)
{
	free(a_row.cells);
}

struct board
{
	struct row* rows;
	char row_nb;
};
typedef struct board board;

// Look for error handling possible when allocating memory, could it be usefull to return something?
void new_board(struct board* a_board, char row_nb)
{
	a_board->rows = malloc(row_nb * sizeof *(a_board->rows));
	a_board->row_nb = row_nb;
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
void delete_board(struct board a_board)
{
	char i;
	for(i = 0; i < a_board.row_nb; i++)
		delete_row(a_board.rows[i]);
	free(a_board.rows);
}

struct game
{
	struct player* players;
	struct board board;
	char player_nb;
	int turn_time;
	char point_to_win;
};
typedef struct game game;

void new_game(struct game* a_game, struct player* players, struct board

int main(void)
{
	return 0;
}
