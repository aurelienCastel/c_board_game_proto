#**Token type**
---
struct hero
{
	char* name;
	struct token_type* token_type;
};

---
The data structure token_type represent a type of token,
A token_type have:
* a name
* a symbol to represented it

An actual token that can be played on the board contains a reference to a token_type.
A hero contains a reference to a token_type.
Token's types shouldn't be create in game, they should be saved as some data.
New token_type should be declared in token_types.h and defined in token_types.c.
