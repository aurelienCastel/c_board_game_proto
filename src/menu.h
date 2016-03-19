#ifndef menu_h
#define menu_h

#include <stdint.h>

#include "game.h"
#include "console_color.h"

void main_menu();

void pvp_menu();
void pve_menu();

void launch_game(struct game* game);

void bad_input_message();

char* player_name_request(int8_t player_number, struct player players[]);
struct hero* hero_request(int8_t player_number, uint8_t is_ai);
color color_request(int8_t player_number, struct player players[]);
struct board_model* board_request();
uint16_t time_request();
uint8_t points_request();

uint8_t is_color_taken(color color, int8_t player_number, struct player players[]);
uint8_t is_name_taken(char* name, int8_t player_number,struct player players[]);

void print_heroes();
void print_colors();
void print_boards();

struct hero* get_hero_ref(char* hero_name);
struct board_model* get_board_model_ref(char* board_name);

#endif
