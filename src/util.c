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

char* int_to_ordinal(uint8_t integer)
{
	switch(integer)
	{
		case 1:
			return "first";
			break;
		case 2:
			return "second";
			break;
		case 3:
			return "third";
			break;
		case 4:
			return "forth";
			break;
		case 5:
			return "fifth";
			break;
		case 6:
			return "sixth";
			break;
		case 7:
			return "seventh";
			break;
		case 8:
			return "eighth";
			break;
		default:
			return "?";
			break;
	}
}
