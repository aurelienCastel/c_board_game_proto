#include "util.h"

#include<stdlib.h>
#include<stddef.h>
#include <stdio.h>

#define LONGEST_INPUT 255

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
	uint8_t i = 0;

	while(string[i] == string_2[i])
	{
		if(string[i] == '\0')
			return 1;
	}

	return 0;
}

void store_input(char* storage)
{
	uint8_t i;

	for(i = 0; (storage[i] = getchar()) != '\n' && i < LONGEST_INPUT; i++);

	if(storage[i] != '\n')
		while(getchar() != '\n');

	storage[i] = '\0';
}
