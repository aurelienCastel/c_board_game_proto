#include "util.h"

#include<stdlib.h>
#include<stddef.h>
#include <stdio.h>

uint8_t string_length(char* string)
{
	uint8_t i;

	for(i = 0; string[i] != '\0'; i++);

	return i;
}

char* string_copy(char* string, uint8_t length)
{
	uint8_t i;

	char* new_string = malloc(length + 1 * sizeof(*new_string));
	
	for(i = 0; i <= length; i++)
		new_string[i] = string[i];

	return new_string;
}

uint8_t are_string_equal(char* string, char* string_2)
{
	uint8_t i;

	for(i = 0; string[i] == string_2[i]; i++)
	{
		if(string[i] == '\0')
			return 1;
	}

	return 0;
}

char* get_input(uint8_t max_length)
{
	uint8_t i;

	char* input = malloc(max_length + 1 * sizeof(*input));

	for(i = 0; (input[i] = getchar()) != '\n' && i <= max_length; i++);

	if(input[i] != '\n')
		while(getchar() != '\n');

	input[i] = '\0';

	return input;
}

void bad_input_message()
{
	printf("Please type a valid choice and press enter.\n");
}

int8_t is_digit(char character)
{
	return character >= '0' && character <= '9';
}

int8_t char_to_int(char character)
{
	return character - 48;
}

char* int_to_ordinal(uint8_t integer)
{
	switch(integer)
	{
		case 1:
			return "first";
		case 2:
			return "second";
		case 3:
			return "third";
		case 4:
			return "forth";
		case 5:
			return "fifth";
		case 6:
			return "sixth";
		case 7:
			return "seventh";
		case 8:
			return "eighth";
		default:
			return "?";
	}
}
