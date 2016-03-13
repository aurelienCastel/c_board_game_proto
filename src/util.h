#ifndef util_h
#define util_h

#include <stdint.h>

uint8_t string_length(char* string);
char* string_copy(char* string, uint8_t length);
uint8_t are_string_equal(char* string, char* string_2);

#endif
