#include <stdlib.h>
#include <stddef.h>

#include "hero.h"

// Look for error handling possible when allocating memory, could it be usefull to return something?
// Maybe we will add special_skill and token_nb_start to this constructor.
// The hero own the token_type, once the hero is deleted, the token_type is deleted too.
void init_hero(struct hero* a_hero, char* name, struct token_type* token_type)
{
	a_hero->name = name; //string_copy(name, string_length(name))
	a_hero->token_type = token_type;
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
//void delete_hero(struct hero a_hero)
//{
//	free(a_hero.name);
//}
// If we add special_skill to it we will maybe need a constructor and destructor.
