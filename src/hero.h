#ifndef hero_h
#define hero_h

struct hero
{
	char* name;
	struct token_type* token_type;
};

void init_hero(struct hero* a_hero, char* name, struct token_type* token_type);

//void delete_hero(struct hero a_hero);

#endif
