#include<stdlib.h>
#include<stddef.h>
#include <stdint.h>

#include "util.h"
#include "general_debug.h"

int string_length(char* string)
{
	handle_error(string == NULL, "string_length cannot calculate the length of a null pointer.");

	int i;

	for(i = 0; string[i] != '\0'; i++);

	return i;

	error:
		return -1;
}

char* string_copy(char* string, unsigned int length)
{
	handle_error(string == NULL, "string_copy cannot copy a null pointer.");

	char* new_string = malloc(length + 1 * sizeof(*new_string));
	unsigned int i;

	handle_mem_error(new_string == NULL);
	
	for(i = 0; i < length; i++)
		new_string[i] = string[i];

	new_string[i] = '\0';

	return new_string;

	error:
		return NULL;
}

uint8_t are_string_equal(char* string, char* string2)
{
	handle_error(string == NULL, "are_string_equal cannot compare null pointer 'string'.");
	handle_error(string2 == NULL, "are_string_equal cannot compare null pointer 'string2'.");

	int i = 0;

	while(string[i] == string2[i])
	{
		if(string[i] == '\0')
			return 1;
	}

	return 0;

	error:
		return 0;
}
