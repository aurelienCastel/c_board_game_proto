#**Board**

---
```
struct cell
{
	uint8_t is_hole;
	uint8_t is_border;
	struct token* token;
};

struct row_info
{
	uint8_t limit_left;
	uint8_t limit_right;
};

struct board
{
	char* name;
	struct cell** grid;
	uint8_t row_nb;
	struct row_info* row_info;
};
```

---
The data structure board represent a board on which players can play.
A board have:
* a name (this is usefull to select a board before the game start)
* a grid, to store all the cells in a 2D manner
* the number of rows on the grid
* an array that contains the index of the leftmost and rightmost cell of each row

The row_info and cell structures shouldn't be created by themselves, the are supposed
to be made during a board creation with the init_board function.

---
#####`void init_board(struct board* board, char* name, uint8_t row_nb, char* model[])`:

Initialize a board structure with the values given as parameters.
The `model` parameter is an array of string that is a rough visual representation of the board.
Prerequisite:
* `board` must be an unitialized board structure
* the name of the board must be unique among all boards
* `name` must be less than 256 character long
* `row_nb` must be equal to the number of string in `model`
* `model` must have less than 256 strings
* strings in `model` must be less than 256 characters long

* `model` looks like this: 

```
(char*[]){"* ***",
		  " **",
		  "**"}
```

strings of `model` only contains spaces and * characters, * represent a cell,
a space represent a cell that is a hole.
Depending on who you want to align rows you may want to puts holes at the beginning of the strings.
You should however never put holes at the end of the string, the last character of any string must be a *.
The `model` above will create this grid:

```
	----    ------------
	|   |   |   |   |   |
	--------------------
	    |   |   |
	------------
	|   |   |
	--------
```

*Once you finished using the board initialized by this function, use the `delete_board` function on it.*

---
#####`void delete_board(struct board board)`:

Free memory taken by the `malloc-ed` properties of `board`
Prerequisite:
* `board` need to be an initialized board structure

---
#####`void print_board(struct board* board)`:

Display the board on the console.
Prerequisite:
* `board` need to be an initialized board structure
