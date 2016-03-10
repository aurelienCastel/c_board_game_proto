@echo off

mkdir test_bin
pushd test_bin
cl -W4 -D_CRT_SECURE_NO_WARNINGS -DERROR_HANDLING c:/fun/src/util.c c:/fun/src/token.c c:/fun/src/hero.c c:/fun/src/player.c c:/fun/src/board.c c:/fun/test/test.c
popd
