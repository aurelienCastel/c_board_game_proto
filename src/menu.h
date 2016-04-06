#ifndef menu_h
#define menu_h

#include <stdint.h>

#include "game.h"
#include "console_color.h"

void main_menu();

void pvp_menu();
void pve_menu();

void print_boards();

uint8_t is_name_taken(char* name, int8_t player_number,struct player players[]);
char* player_name_request(int8_t player_number, struct player players[]);

uint8_t is_color_taken(color color, int8_t player_number, struct player players[]);
color color_request(int8_t player_number, struct player players[]);

struct board_model* get_board_model_ref(char* board_name);
struct board_model* board_request();

uint16_t time_request();
uint8_t points_request();

#endif
