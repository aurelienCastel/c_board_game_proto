#include <stdlib.h>
#include <stddef.h>

#include "token.h"

void init_token(struct token* token, struct player* player, struct token_type* type)
{
	token->owner = player;
	token->type = type;
}
