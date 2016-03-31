#ifndef util_h
#define util_h

#include <stdint.h>

uint8_t string_length(char* string);
char* string_copy(char* string, uint8_t length);
uint8_t are_string_equal(char* string, char* string_2);

char* get_input(uint8_t max_length);
void bad_input_message();

int8_t is_digit(char character);
int8_t char_to_int(char character);
char* int_to_ordinal(uint8_t integer);

#endif
