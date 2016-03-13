# **Util**

---
####uint8_t string_length(char* _string_) : 
Returns the number of characters in _string_, the final '\0' character is not counted.
The number returned is a positive integer between 0 and 255 (both included).
Prerequisite: 
* _string_ must be less than 256 character long
* _string_ must be null terminated

---
####char* string_copy(char* _string_, uint8_t _length_):
Returns a copy of the string _string_, the string returned is allocated with the malloc function.
Prerequisite:
* _string_ must be less than 256 character long
* _string_ must be null terminated
* _length_ must be the number of character in _string_, the final '\0' character musn't   be counted
**Once you finished using the string returned by this function, use the free function on it.**

---
####uint8_t are_string_equal(char* _string_, char* _string\_2_):
Returns 1 if _string_ and  _string\_2_ are made with the same characters, otherwise it returns 0.
Prerequisite:
* _string_ and _string\_2_ must be less than 256 character long
* _string_ and _string\_2_ must be null terminated



