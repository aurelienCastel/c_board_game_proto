#include<stdio.h>

#include "menu.h"
#include "util.h"

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
}

void custom_game_menu()
{
}

