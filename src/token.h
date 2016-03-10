#ifndef token_h
#define token_h

struct player;

struct token
{
	struct player* owner;
	struct token_type* type;
};

void init_token(struct token* a_token, struct player* player, struct token_type* type);

//void delete_token(struct token a_token);

#endif
