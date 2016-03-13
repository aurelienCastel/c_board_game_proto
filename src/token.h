#ifndef token_h
#define token_h

struct player;

struct token
{
	struct player* owner;
	struct token_type* type;
};

void init_token(struct token* token, struct player* player, struct token_type* type);

#endif
