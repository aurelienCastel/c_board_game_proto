#ifndef util_h
#define util_h

#include <stdint.h>

uint8_t string_length(char* string);
uint8_t longest_string(char* strings[], uint8_t array_length);
char* string_copy(char* string, uint8_t length);
uint8_t strings_are_equal(char* string, char* string_2);

char* get_input(uint8_t max_length);
void bad_input_message();

uint8_t is_digit(char character);
uint8_t char_to_int(char character);
char* int_to_ordinal(uint8_t integer);

#endif
