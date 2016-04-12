#include<stdio.h>
#include<stdlib.h>
#include<stddef.h>

#include "menu.h"
#include "util.h"
#include "player.h"
#include "boards.h"
#include "game.h"
#include "console_color.h"

void main_menu()
{
	char* input;

	do
	{
		printf("1 - Player vs player\t2 - Player vs ai\n"
			   "3 - Quit\n"
			   "Enter your choice:");
		input = get_input(1);

		switch(input[0])
		{
			case '1':
				pvp_menu();
				break;
			case '2':
				pve_menu();
				break;
			case '3':
				printf("Goodbye.");
				break;
			default:
				free(input);
				bad_input_message();
		}
	}while(input[0] != '3');
}

void pvp_menu()
{
	struct player player1;
	struct player player2;
	struct player* players[2];
	struct board board;
	struct game game;

	init_player(&player1, &player2, "player1", 0, FG_BLUE);
	init_player(&player2, &player1, "player2", 0, FG_RED);
	players[0] = &player1;
	players[1] = &player2;

	init_board(&board, &Board_7x7_hole_center);

	init_game(&game, players, &board, 3, 15);

	launch_game(&game);
}

void pve_menu()
{
	struct player* players[2];

	struct board board;
	struct game game;

	init_player(players[0], players[1], player_name_request(-1, NULL), 0, FG_BLUE);
	init_player(players[1], players[0], "AI", 1, FG_RED);

	init_board(&board, board_request());

	init_game(&game, players, &board, 5, 15);

	launch_game(&game);
}

void print_boards()
{
	uint8_t i;

	for(i = 0; Boards[i] != NULL; i++)
	{
		printf("%s\n", Boards[i]->name);
	}
}

uint8_t is_name_taken(char* name, int8_t player_number, struct player players[2])
{
	uint8_t i;

	for(i = 0; i < player_number; i++)
	{
		if(strings_are_equal(name, players[i].name))
			return 1;
	}
	return 0;
}

char* player_name_request(int8_t player_number, struct player players[2])
{
	char* name;

	while(1)
	{
		if(player_number == -1)
			printf("Enter your name:");
		else
			printf("Enter %s player's name:", int_to_ordinal(player_number + 1));

		name = get_input(30);

		if(name[0] == '\0')
		{
			free(name);
			bad_input_message();
			continue;
		}

		if(!is_name_taken(name, player_number, players))
			return name;
		else
		{
			free(name);
			printf("This name is already taken by another player.\n");
		}
	}
}

uint8_t is_color_taken(color color, int8_t player_number, struct player players[2])
{
	uint8_t i;

	for(i = 0; i < player_number; i++)
	{
		if(color == players[i].color)
			return 1;
	}
	return 0;
}

color color_request(int8_t player_number, struct player players[2])
{
	char* input;
	int color;

	while(1)
	{
		printf("1 - Blue\t 2 - Red\t 3 - Green\n"
			   "4 - Cyan\t 5 - Magenta\t 6 - Black\n"
			   "Enter your choice:");
		input = get_input(1);
		color = atoi(input);
		free(input);

		switch(color)
		{
			case 1:
				color = FG_BLUE;
				break;
			case 2:
				color = FG_RED;
				break;
			case 3:
				color = FG_GREEN;
				break;
			case 4:
				color = FG_CYAN;
				break;
			case 5:
				color = FG_MAGENTA;
				break;
			case 6:
				color = FG_BLACK;
				break;
			default:
				bad_input_message();
				continue;
		}

		if(!is_color_taken(color, player_number, players))
			return color;
		else
			printf("This color is already taken by another player.\n");
	}
}

struct board_model* get_board_model_ref(char* board_name)
{
	uint8_t i;

	for(i = 0; Boards[i] != NULL; i++)
	{
		if(strings_are_equal(board_name, Boards[i]->name))
			return Boards[i];
	}

	return NULL;
}

struct board_model* board_request()
{
	char* name;
	struct board_model* board_model;

	while(1)
	{
		print_boards();
		printf("Enter the name of the board you want to play on:");
		name = get_input(60);
		board_model = get_board_model_ref(name);
		free(name);

		if(board_model != NULL)
			return board_model;
		else
			bad_input_message();
	}
}

uint16_t time_request()
{
	char* input;
	int time;

	while(1)
	{
		printf("1 - Short time (15 seconds)\t 2 - Medium time (30 seconds)\t 3 - Long time (60 seconds)\n"
			   "Enter your choice:");
		input = get_input(1);
		time = atoi(input);
		free(input);

		switch(time)
		{
			case 1:
				return 15;
			case 2:
				return 30;
			case 3:
				return 60;
			default:
				bad_input_message();
		}
	}
}

uint8_t points_request()
{
	char* input;
	int points;

	while(1)
	{
		printf("1 - 5 points\t 2 - 7 points\t 3 - 9 points\n"
			   "Enter your choice:");
		input = get_input(1);
		points = atoi(input);
		free(input);

		switch(points)
		{
			case 1:
				return 5;
			case 2:
				return 7;
			case 3:
				return 9;
			default:
				bad_input_message();
		}
	}
}
