#include <stddef.h>
#include <stdio.h>

#include "util.h"
#include "console_color.h"
#include "token_types.h"
#include "heroes.h"
#include "player.h"
#include "board.h"
#include "boards.h"
#include "menu.h"

int main(void)
{
	//main_menu();

	int i;
	for(i = 0; Heroes[i] != NULL; i++)
		printf("%s\n", Heroes[i]->name);

	for(i = 0; Boards[i] != NULL; i++)
		printf("%s\n", Boards[i]->name);

	return 0;
}
