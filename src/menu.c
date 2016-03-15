#include<stdio.h>

#include "menu.h"
#include "util.h"

// Maybe all menu functions can return the following function (to free the memory on the stack).

void main_menu()
{
	char* input = "";

	while(input[0] != '3')
	{
		printf("1 - Standard game\t2 - Custom game\n"
			   "3 - Quit\n"
			   "Enter your choice:");
		store_input(input);

		switch(input[0])
		{
			case '1':
				standard_game_menu();
				break;
			case '2':
				custom_game_menu();
				break;
			case '3':
				printf("Goodbye.");
				break;
			default:
				printf("Please type a valid choice and press enter.\n");
		}
	}
}

void standard_game_menu()
{
	char* input = "";

	while(input[0] < '1' || input[0] > '3')
	{
		printf("1 - Player vs player\t2 - Player vs AI\n"
			   "3 - Return to main menu\n"
			   "Enter your choice:");
		store_input(input);

		switch(input[0])
		{
			case '1':
				pvp_menu();
				break;
			case '2':
				pve_menu();
				break;
			case '3':
				break;
			default:
				printf("Please type a valid choice and press enter.\n");
		}
	}

}

void pvp_menu()
{
	char* player_name = "";
	char* player_hero = "";
	char* ai_hero = "";
	char* time = "";
	char* board = "";

	printf("Enter your name:");
	store_input(player_name);

	while(1)
	{
		// Add a print with all playable heroes.
		printf("Enter the name of the hero you want to play:");
		store_input(player_hero);

		if()
			break;
		else
			printf("Please type a valid choice and press enter.\n");

	}

	while(1)
	{
		// Add a print with all playable heroes.
		printf("Enter the name of your oponent's hero:");
		store_input(ai_hero);

		if()
			break;
		else
			printf("Please type a valid choice and press enter.\n");
	}

	while(1)
	{
		printf("1 - Short time | 5 points\t 2 - Medium time | 5 points\t 3 - Long time | 5 points\n"
			   "4 - Short time | 7 points\t 5 - Medium time | 7 points\t 6 - Long time | 7 points\n"
			   "7 - Short time | 9 points\t 8 - Medium time | 9 points\t 9 - Long time | 9 points\n"
			   "Enter your choice:");
		store_input(time);

		if(time[0] >= '1' && time[0] <= 9)
			break;
	}

	while(1)
	{
		// Add a print with all playable boards 
		printf("Enter the name of the board you want to play on:");
		store_input(board);

		if()
			break;
		else
			printf("Please type a valid choice and press enter.\n");
	}

	// Lauch a game with all collected data.

}

void pve_menu()
{
}

void custom_game_menu()
{
}

