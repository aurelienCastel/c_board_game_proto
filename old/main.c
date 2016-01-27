// TODO :
// enum tokens:   Maybe i can remove this and use adresses of globals token to difference empty from forbidden...
// handle_input():  Maybe if the function return a string a can get rid of the stack issue.
// coin_state() / coin_flip() : Change this to a something that randomize players order automatically.
// free the memory.

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>

#define LONGEST_INPUT 20

// Data definition.

enum tokens
{
	empty, player1, player2, forbidden
};

enum games_states
{
	start, help, tuto, coin, match
};

struct coord
{
	int y;
	int x;
};

struct token
{
	char type;
	char representation;
};

struct player
{
	int is_human;
	int order;
	struct token** tokens;
	struct token* selected_token;
};

struct board
{
	int y_size;
	int x_size;

	struct token*** matrix;
};

struct game
{
	struct board board;
	int nb_player;
	struct player* players;
	struct player* current_player;
};

struct state
{
	int nb_previous;
	int* previous_stack;
	int current;
	int previous;
};

// Functions.

struct player create_player(int is_human, struct token** tokens);

struct board create_board(int x_size, int y_size);
void display_board();

struct game create_game();

struct coord create_coord(int y, int x);

int is_coordinate(char* string);
struct coord string_to_coordinate(char* string);
int is_valid_move(struct coord cd);
struct coord valid_rand_coord();

void go_state(int game_state);
void help_state();
void tuto_state();
void coin_state();
void match_state();

void coin_flip(char* choice);
void ask_move();
void play_move(struct coord cd);
void go_to_next_player();
int match_winner();
void leave_game();

void input_request(char* message);
void store_input(char* storage);
void handle_input();
void re_ask_input();

void intro_message();
void tuto_message();

int are_string_equal(char* string, char* string2);
int is_digit(char c);

// All differents tokens
struct token Empty = {empty, ' '};
struct token Player1 = {player1, 'o'};
struct token Player2 = {player2, 'x'};
struct token Forbidden = {forbidden, ' '};

struct game* Game;
struct game MainGame;
struct state State;

int main(int argc, char* argv[])
{ 
	srand(time(NULL));

	MainGame = create_game();
	Game = &MainGame;

	State.nb_previous = 0;
	State.previous_stack = malloc(State.nb_previous * sizeof *(State.previous_stack));
	State.current = start;
	State.previous = start;

	intro_message();
	go_state(coin);
	
	return 0;
}

struct player create_player(int is_human, struct token** tokens)
{
	struct player pl;
	pl.is_human = is_human;
	pl.tokens = tokens;
	pl.selected_token = pl.tokens[0];
	return pl;
}

struct board create_board(int x_size, int y_size)
{
	struct board board;

	board.x_size = x_size;
	board.y_size = y_size;

	board.matrix = malloc(y_size * sizeof *board.matrix);

	int y, x;
	for(y = 0; y < y_size; y++)
		*((board.matrix) + y) = malloc(y_size * sizeof **board.matrix);
	for(y = 0; y < y_size; y++)
	{
		for(x = 0; x < x_size; x++)
			board.matrix[y][x] = &Empty;
	}

	return board;
}

void display_board()
{
	struct board* board = &(Game->board);
	int y, x;

	printf("\n");
	for(y = 0; y < board->y_size; y++)
	{
		for(x = 0; x <= (board->x_size * 4); x++)
			printf("-");
		printf("\n");

		printf("|");

		for(x = 0; x < board->x_size; x++)
		{
			printf(" %c ", board->matrix[y][x]->representation);
			printf("|");
		}

		printf("\n");
	}
	for(x = 0; x <= (board->x_size * 4); x++)
		printf("-");
	printf("\n");
}

struct game create_game()
{
	struct game gm;

	gm.board = create_board(4, 4);
	gm.nb_player = 2;
	gm.players = malloc(gm.nb_player * sizeof *(gm.players));
	
	struct token* token_array1[] = {&Player1};
	gm.players[0] = create_player(0, token_array1);

	struct token* token_array2[] = {&Player2};
	gm.players[1] = create_player(0, token_array2);

	return gm;
}

struct coord create_coord(int y, int x)
{
	struct coord cd;
	cd.y = y;
	cd.x = x;
	return cd;
}

int is_coordinate(char* string)
{
	if(*string == '-')
		string++;
	while(*string != ' ')
	{
		if(!is_digit(*string))
			return 0;
		string++;
	}
	string++;

	if(*string == '-')
		string++;
	while(*string != '\0')
	{
		if(!is_digit(*string))
			return 0;
		string++;
	}
	return 1;
}

struct coord string_to_coordinate(char* string)
{
	struct board* board = &(Game->board);
	int y = 0, x = 0;
	int y_negative = 0, x_negative = 0;

	if(*string == '-')
	{
		y_negative = 1;
		string++;
	}
	while(*string != ' ')
	{
		y *= 10;
		y += *string - '0';
		string++;
	}
	if(y_negative) 
		y = board->y_size - y;
	else
		y -= 1;

	string++;
	if(*string == '-')
	{
		x_negative = 1;
		string++;
	}
	while(*string != '\0')
	{
		x *= 10;
		x += *string - '0';
		string++;
	}
	if(x_negative) 
		x = board->x_size - x;
	else
		x -= 1;

	return create_coord(y, x);
}

int is_valid_move(struct coord cd)
{
	struct board* board = &(Game->board);

	if(cd.y >= 0 && cd.x >= 0 &&
		cd.y < board->y_size && cd.x < board->x_size)
	{
		if(board->matrix[cd.y][cd.x]->type == empty)
			return 1;
	}
	return 0;
}

struct coord valid_rand_coord()
{
	struct board* board = &(Game->board);
	struct coord cd;

	do
	{
		cd.y = rand() % board->y_size;
		cd.x = rand() % board->x_size;
	}while(board->matrix[cd.y][cd.x]->type != empty);

	return cd;
}

void go_state(int game_state)
{
	if(game_state == State.previous)
	{
		State.nb_previous--;
		State.previous_stack = realloc(State.previous_stack, State.nb_previous * sizeof *(State.previous_stack));

		State.current = game_state;
		State.previous = State.previous_stack[State.nb_previous - 1];
	}

	else if(game_state != State.current)
	{
		State.nb_previous++;
		State.previous_stack = realloc(State.previous_stack, State.nb_previous * sizeof *(State.previous_stack));
		State.previous_stack[State.nb_previous - 1] = State.current;

		State.previous = State.current;
		State.current = game_state;
	}

	Game = &MainGame;
	switch(game_state)
	{
		case help:
			help_state();
			return;
		case tuto:
			tuto_state();
			return;
		case coin:
			coin_state();
			return;
		case match:
			match_state();
			return;
	}
}

void help_state()
{
	char* input;

	printf("\nYou are in the help menu.\n"
	"To learn who to play the game type \"tuto\".\n"
	"To return to the game type \"return\".\n"
	"To quit the game type \"quit\".\n");

	input_request("choice");
	handle_input();
}

void tuto_state()
{
	int x, y;
	struct game gm;
	struct player pl;
	pl.selected_token = &Player1;
	gm.board = create_board(4, 4);
	gm.current_player = &pl;

	for(y = 0; y < gm.board.y_size; y++)
	{
		for(x = 0; x < gm.board.x_size; x++)
		{
			if(y != 0 || x != 0)
				gm.board.matrix[y][x] = &Forbidden;
		}
	}

	Game = &gm;

	tuto_message();

	display_board();
	ask_move();

	printf("Well done!\n"
			"Now put one on the third square of the second row.\n");
	gm.board.matrix[1][2] = &Empty;

	ask_move();

	printf("Alright.\n"
			"Using positive coordinates only can be a bit painful when playing on a big board.\n"
			"Use negative coordinates to put a token on the last square of the last row.\n");
	gm.board.matrix[3][3] = &Empty;

	ask_move();

	printf("That's it. You know everything you need to play.\n");
	go_state(State.previous);
}

void coin_state()
{
	char* input;
	printf("\nYou are going to play against the computer, to know who plays first choose \"head\" or \"tail\".\n");

    input_request("choice");
	handle_input();
	go_state(match);
}

void match_state()
{
	int winner;

	display_board();
	do
	{
		ask_move();
		winner = match_winner();
		go_to_next_player();
	}while(!winner);

	if(Game->players[winner - 1].is_human)
		printf("You won, congratulation!\n");
	else
		printf("The computer won, better luck next time.\n");
	leave_game();
}

void coin_flip(char* choice)
{
	int coin = rand() % 2;
	char* result;
	if(coin)
		result = "head";
	else
		result = "tail";

	if(are_string_equal(choice, result)) 
	{
		Game->players[0].is_human = 1;
		printf("Vous jouez en premier.\n");
	}
	else
	{
		Game->players[1].is_human = 1;
		printf("L'ordinateur joue en premier.\n");
	}
	Game->current_player = Game->players;
}

void ask_move()
{
	input_request("move");
	handle_input();
	display_board();
}

void play_move(struct coord cd)
{
	Game->board.matrix[cd.y][cd.x] = Game->current_player->selected_token;
}

void go_to_next_player()
{
	if(Game->current_player == Game->players)
		Game->current_player = Game->players + 1;
	else
		Game->current_player = Game->players;
}

int match_winner()
{
	int winner = 0;
	int succession = 0;
	struct token* last_token;

	int* players = malloc(Game->nb_player * sizeof *players);
	int x, y;

	for(x = 0; x < Game->board.x_size; x++)
	{
		last_token = &Empty;
		succession = 0;
		for(y = 0; y < Game->board.y_size; y++)
		{
			if(Game->board.matrix[x][y] == last_token)
			{
				succession++;
			}
			else
			{
				succession = 0;
				last_token = Game->board.matrix[x][y];
			}
			if(succession == 2)
			{
				if(last_token == &Player1)
					return 1;
				else if(last_token == &Player2)
					return 2;
			}
		}
	}
	
	for(y = 0; y < Game->board.y_size; y++)
	{
		last_token = &Empty;
		succession = 0;
		for(x = 0; x < Game->board.x_size; x++)
		{
			if(Game->board.matrix[x][y] == last_token)
			{
				succession++;
			}
			else
			{
				succession = 0;
				last_token = Game->board.matrix[x][y];
			}
			if(succession == 2)
			{
				if(last_token == &Player1)
					return 1;
				else if(last_token == &Player2)
					return 2;
			}
		}
	}

	succession = 0;
	last_token = &Empty;
	for(x = 0; x < Game->board.x_size; x++)
	{
		if(Game->board.matrix[x][x] == last_token)
		{
			succession++;
		}
		else
		{
			succession = 0;
			last_token = Game->board.matrix[x][x];
		}
		if(succession == 2)
		{
			if(last_token == &Player1)
				return 1;
			else if(last_token == &Player2)
				return 2;
		}
	}

	succession = 0;
	last_token = &Empty;
	for(x = 0; x < Game->board.x_size; x++)
	{
		if(Game->board.matrix[x][Game->board.x_size - x - 1] == last_token)
		{
			succession++;
		}
		else
		{
			succession = 0;
			last_token = Game->board.matrix[x][Game->board.x_size - x - 1];
		}
		if(succession == 2)
		{
			if(last_token == &Player1)
				return 1;
			else if(last_token == &Player2)
				return 2;
		}
	}

	return 0;
}

void leave_game()
{
	printf("Good bye.\n");
	exit(0);
}

void input_request(char* message)
{
	printf("Enter your %s:", message);
}

void store_input(char* storage)
{
	int i;

	for(i = 0; (storage[i] = getchar()) != '\n' && i < LONGEST_INPUT; i++);
	storage[i] = '\0';
}

void handle_input()
{
	char input[LONGEST_INPUT];
	store_input(input);
	struct coord cd;

	switch(State.current)
	{
		case help:
			if(are_string_equal(input, "tuto"))
			{
				go_state(tuto);
				return;
			}
			if(are_string_equal(input, "return"))
			{
				go_state(State.previous);
				return;
			}
			break;
		case tuto:
			if(are_string_equal(input, "return"))
			{
				go_state(State.previous);
				return;
			}
			if(is_coordinate(input) && is_valid_move(cd = string_to_coordinate(input)))
			{
				play_move(cd);
				return;
			}
			break;
		case coin:
			if(are_string_equal(input, "head") || are_string_equal(input, "tail"))
			{
				coin_flip(input);
				return;
			}
			break;
		case match:
			if(is_coordinate(input) && is_valid_move(cd = string_to_coordinate(input)))
			{
				play_move(cd);
				return;
			}
			break;
	}
	if(are_string_equal(input, "help"))
	{
		go_state(help);
		return;
	}
	if(are_string_equal(input, "quit"))
		leave_game();

	re_ask_input();
	handle_input();
}

void re_ask_input()
{
	printf("Invalid input.\n"
	"Type a valid text and push the Enter key.\t(Exemple: ");
	switch(State.current)
	{
		case help:
			printf("tuto)\n");
			input_request("choice");
			break;
		case tuto:
		case match:
			struct coord rand_coord = valid_rand_coord();
			printf("%d %d)\n", rand_coord.y + 1, rand_coord.x + 1);
			input_request("move");
			break;
		case coin:
			printf("head)\n");
			input_request("choice");
			break;
	}
}

void intro_message()
{
	printf("\nWelcome in Tic Tac Toe Advanced.\n"
	"For any information or to go through a tutorial type \"help\".\n");
}

void tuto_message()
{
	printf("\nIn Tic Tac Toe Advanced, you put tokens on a board.\n"
	"To win you have to fill a collumn, a row or a diagonal with your tokens only.\n"
	"Each player can put one token every turn.\n"
	"Players cannot put a token where there is already one.\n");

	printf("\nTo put a token on the board you have to type its coordinates and push the enter key.\n"
	"First type the number of the row you want to put the token on.\n"
	"Then type a space.\n"
	"Then type the number of the square you want to put the token on.\n"
	"For exemple if you want to put a token on the third square of the second row type: 2 3\n");

	printf("\nThere is two way to write coordiantes:\n");

	printf("\tWith positive numbers:\n"
				"\t\tRows are counted from top to bottom.\n"
				"\t\tSquares are counted from left to right.\n"
			"\tWith negative numbers:\n"
				"\t\tRows are counted from bottom to top.\n"
				"\t\tSquares are counted from right to left.\n");
	printf("\nLet's give it a try:\n"
			"Here is an empty board, put a token on the first square of the first row.\n");
}

int are_string_equal(char* string, char* string2)
{
	while(1)
	{
		if(*string == *string2)
		{
			if(*string == '\0')
				return 1;
		}
		else
			break;

		string++, string2++;
	}
	return 0;
}

int is_digit(char c)
{
	return (c >= '0' && c <= '9');
}
