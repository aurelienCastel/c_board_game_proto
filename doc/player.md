#**Player**

---

```
struct player
{
	char* name;
	uint8_t is_ai;
	uint8_t number;
	uint8_t team;
	cc_color_t color;
	struct hero* hero;
	struct token token;
	struct token special_token;
	uint8_t special_token_amount;
	uint8_t is_special_skill_available;
	uint8_t points;
};
```

---
The data structure player represent a player taking part in a game.
A player have:
* a name
* an indicator to tell if it is an ai
* a number to indicate in which order he/she plays
* the number of his/her team
* a color to differentiate his/her token
* a reference to the hero he/she is playing
* the standard token he/she can play
* the special token he/she can play specified by his/her hero
* the amount of special token aviable
* an indicator to tell if the player can use the special skill of his/her hero
* his/her points in the current game

Human players and computer players use the same structure.
Players are created before the start of the game, from informations given by the user in the lobby phase.
Players can't have the same name. They can have the same color only if they are in the same team.
There is  alot of dependencies between the players, the board and the game conditions (points to win, time to play), elements of data in structure and/or algorithm must prevent users to create incompatible games.
The order of players is determined randomly after the lobby phase.

---

#####`void init_player(struct player* player, char* name, uint8_t is_ai, uint8_t team, color color, struct hero* hero_played)`:

Initialize a player structure with the values given as parameters.
The `number` property of the player is not initialized, it is decided once a game involving this player start.
The initialization of the `name` property use the malloc function.
The initialization `special_token` and `special_token_amount` properties depends on the `hero_played` parameter.
The `is_special_skill_available` and `points` properties are both initialized at zero.
Prerequisite:
* `player` must be a pointer to an uninitialized player structure
* `name` must be less than 256 character long
* `hero_played` must be an initialized hero
* `color` have to be a valid color (according to the colors defined by the enumeration in `console_color.h`)

*Once you finished using the player initialized by this function, use the `delete_player` function on it.*

---
#####`void delete_player(struct player player)`:
Free memory taken by the malloc-ed properties of _player_
Prerequisite:
* `player` need to be an initialized player structure
