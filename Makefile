
SRC		= md5_run.c md5.c
FLAGS	= -O0 -g3 -Wall -Wextra -std=c11 -fdiagnostics-color=always
INCLUDE	= -I .

all:
	clang --version
	clang ${FLAGS} ${INCLUDE} -D __debug ${SRC} -o md5

release:
	clang ${FLAGS} ${INCLUDE} ${SRC} -o md5

