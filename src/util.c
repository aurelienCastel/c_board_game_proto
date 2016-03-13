#include<stdlib.h>
#include<stddef.h>
#include <stdint.h>

#include "util.h"

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
