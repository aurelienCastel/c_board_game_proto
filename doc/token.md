#**Token**

---
struct token
{
	struct player* owner;
	struct token_type* type;
};

---
The data structure token represent a token owned by a player that can be placed on the board,
A token have:
* a reference to its owner
* a reference to its type

Token's are created when a player is created, before the start of a game.

---
####void init_token(struct token* _token_, struct player* _player_,
					struct token_type* _type_):
This function is called inside of the ini_player function, it shouldn't be called by itself.
Prerequisite:
* Prerequisite of the init_player function need to be respected

