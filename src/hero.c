#include <stdlib.h>
#include <stddef.h>

#include "hero.h"

void init_hero(struct hero* a_hero, char* name, struct token_type* token_type)
{
	a_hero->name = name; //string_copy(name, string_length(name))
	a_hero->token_type = token_type;
}

