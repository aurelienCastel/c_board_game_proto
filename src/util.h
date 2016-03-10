#ifndef util_h
#define util_h

#include <stdint.h>

int string_length(char* string);
char* string_copy(char* string, unsigned int length);
uint8_t are_string_equal(char* string, char* string2);

#endif
