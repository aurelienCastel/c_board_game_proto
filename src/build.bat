@echo off

mkdir bin
pushd bin
cl -W4 -D_CRT_SECURE_NO_WARNINGS c:/fun/src/util.c c:/fun/src/token.c c:/fun/src/hero.c c:/fun/src/player.c c:/fun/src/board.c c:/fun/src/main.c
popd


