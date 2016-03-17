#ifndef boards_h
#define boards_h

#include <stdint.h>
#include "board.h"

extern struct board_model Board_5x5;
extern struct board_model Board_6x6;
extern struct board_model Board_7x7;
extern struct board_model Board_7x7_hole_center;
extern struct board_model Board_big_diamond;

extern struct board_model* Boards[];

#endif
