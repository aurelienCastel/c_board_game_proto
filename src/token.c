#include <stdlib.h>
#include <stddef.h>

#include "token.h"

// Look for error handling possible when allocating memory, could it be usefull to return something?
void init_token(struct token* a_token, struct player* player, struct token_type* type)
{
	a_token->owner = player;
	a_token->type = type;
}

// Look for error handling possible when freeing memory, could it be usefull to return something?
//void delete_token(struct token a_token)
//{
//	// Send a warning when freeing NULL pointers.
//	free(a_token.type);
//}
