#**Hero**

---
struct hero
{
	char* name;
	struct token_type* token_type;
};

---
The data structure hero represent a hero that can be played.
A hero have:
* a name
* a reference to a token_type

Several heroes can't be named the same but they can have the same token_type.
Players don't directly choose their special token, instead they choose a hero which provide a special token to them thanks to his/her token_type.
Heroes shouldn't be create in game, they should be saved as some data.
New hero should be declared in heroes.h and defined in heroes.c.
