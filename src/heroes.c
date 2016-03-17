#include <stddef.h>

#include "heroes.h"
#include "token_types.h"

//void init_heroes(struct* hero)
//{
//}
//
//void delete_heroes(struct hero)
//{
//}

struct hero Yacine = {.name = "Yacine", .token_type = &Fire_type};
struct hero Bleu = {.name = "Bleu", .token_type = &Electric_type};

struct hero* Heroes[] =
{
	&Yacine,
	&Bleu,
	NULL
};

//init_heroes(heroes);
