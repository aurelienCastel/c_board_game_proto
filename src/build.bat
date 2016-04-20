@echo off

mkdir bin
pushd bin
cl -o main.exe -W4 -D_CRT_SECURE_NO_WARNINGS -DERROR_HANDLING c:/c_board_game_proto/src/console_color.c c:/c_board_game_proto/src/util.c  c:/c_board_game_proto/src/player.c c:/c_board_game_proto/src/board.c c:/c_board_game_proto/src/boards.c c:/c_board_game_proto/src/game.c c:/c_board_game_proto/src/menu.c c:/c_board_game_proto/src/main.c
popd


