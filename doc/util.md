# **Util**

---

#####`uint8_t string_length(char* string)`: 

Returns the number of characters in `string`, the final '\0' character is not counted.
The number returned is a positive integer between 0 and 255 (both included).
Prerequisite: 
* `string` must be less than 256 character long
* `string` must be null terminated

---

#####`char* string_copy(char* string, uint8_t length)`:

Returns a copy of the string `string`, the string returned is allocated with the `malloc` function.
Prerequisite:
* `string` must be less than 256 character long
* `string` must be null terminated
* `length` must be the number of character in _string_, the final '\0' character musn't   be counted
*Once you finished using the string returned by this function, use the `free` function on it.*

---
#####`uint8_t are_string_equal(char* string, char* string_2)`:

Returns 1 if `string` and  `string_2` are made with the same characters, otherwise it returns 0.
Prerequisite:
* `string` and `string_2` must be less than 256 character long
* `string` and `string_2` must be null terminated



