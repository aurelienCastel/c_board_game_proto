#include <stddef.h>

#include "boards.h"

struct board_model Board_5x5 = {.name = "5x5", .row_nb = 5,
	.model = (char*[]) {"*****",
					    "*****",
					    "*****",
					    "*****",
					    "*****"}};
  
struct board_model Board_6x6 = {.name = "6x6", .row_nb = 6,
	.model = (char*[]) {"******",
					    "******",
					    "******",
					    "******",
					    "******",
					    "******"}};

struct board_model Board_7x7 = {.name = "7x7", .row_nb = 7,
	.model = (char*[]) {"*******",
						"*******",
						"*******",
						"*******",
						"*******",
						"*******",
						"*******"}};

struct board_model Board_7x7_hole_center = {.name = "7x7 hole center", .row_nb = 7,
	.model = (char*[]) {"*******",
					    "*******",
					    "*******",
					    "*** ***",
					    "*******",
					    "*******",
					    "*******"}};

struct board_model Board_big_diamond = {.name = "big diamond", .row_nb = 11,
	.model = (char*[]) {"     *",
					    "    ***",
					    "   *****",
					    "  *******",
					    " *********",
					    "***********",
					    " *********",
					    "  *******",
					    "   *****",
					    "    ***",
					    "     *"}};
  
struct board_model* Boards[] =
{
	&Board_5x5,
	&Board_6x6,
	&Board_7x7,
	&Board_7x7_hole_center,
	&Board_big_diamond,
	NULL
};

