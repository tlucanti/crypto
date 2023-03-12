
CFLAGS		=	-Wall -Wextra -Wmost -Wpedantic
COPTIONS	=	-D __debug -O0 -g3
LDFLAGS		= 	-L${BUILD_DIR}
ARCFLAGS	=

CC			=	clang
LD			=	clang
ARC			=	ar rcs
RM			=	rm -f

BUILD_ROOT	=	buildsystem
BUILD_DIR	=	build
NAME		= 	ft_ssl

TARGETS		=
LIBRARIES	=

all: build_all ${NAME}

include ${BUILD_ROOT}/cflags.mk
include ${BUILD_ROOT}/build.mk

include ${BUILD_ROOT}/libcrypto.mk

include ${BUILD_ROOT}/md5_run.mk
include ${BUILD_ROOT}/sha224_run.mk
include ${BUILD_ROOT}/sha256_run.mk
include ${BUILD_ROOT}/sha384_run.mk
include ${BUILD_ROOT}/sha512_run.mk

build_libraries: ${BUILD_DIR} $(addprefix __lib_,$(LIBRARIES))
build_targets: ${BUILD_DIR} $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(TARGETS)))
build_all: build_libraries build_targets

clean_targets: $(addprefix __clean_,$(TARGETS))
clean_libraries: $(addprefix __clean_lib_,$(LIBRARIES))
clean: clean_targets clean_libraries

re: clean build_all ${NAME}

${NAME}: src/main.cpp ${BUILD_DIR}/libcrypto.a
	@printf "${G}[ c++ ] ${W} src/main.o\n"
	@clang++ \
		${CFLAGS} ${LDFLAGS} \
		-std=c++2a \
		src/main.cpp \
		-I include \
		-lcrypto \
		-o ${NAME}

#.PHONY: build_libraries build_targets build clean re
