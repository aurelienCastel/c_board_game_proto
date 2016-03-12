@echo off

mkdir debug
pushd debug
cl -o main.exe -W4 -D_CRT_SECURE_NO_WARNINGS -DERROR_HANDLING c:/fun/src/console_color.c c:/fun/src/util.c c:/fun/src/token_types.c c:/fun/src/hero.c c:/fun/src/heroes.c c:/fun/src/token.c c:/fun/src/player.c c:/fun/src/board.c c:/fun/src/main.c
popd


