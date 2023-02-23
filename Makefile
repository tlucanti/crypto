
CFLAGS		=	-Wall -Wextra -Wmost -Wpedantic
CFLAGS		+=	-D __debug -O0 -g3
LDFLAGS		=
ARCFLAGS	=

CC			=	clang
LD			=	clang
ARC			=	ar rcs
RM			=	rm -f

BUILD_ROOT	=	buildsystem
BUILD_DIR	=	build

TARGETS		=
LIBRARIES	=

include ${BUILD_ROOT}/cflags.mk
include ${BUILD_ROOT}/build.mk

include ${BUILD_ROOT}/libcrypto.mk

include ${BUILD_ROOT}/md5_run.mk
include ${BUILD_ROOT}/sha224_run.mk
include ${BUILD_ROOT}/sha256_run.mk
include ${BUILD_ROOT}/sha384_run.mk
include ${BUILD_ROOT}/sha512_run.mk

build_libraries: ${BUILD_DIR} $(addprefix __lib_,$(LIBRARIES))
build_targets: ${BUILD_DIR} $(addprefix __build_,$(TARGETS))
#build: build_libraries build_targets

clean_targets: $(addprefix __clean_,$(TARGETS))
clean_libraries: $(addprefix __clean_lib_,$(LIBRARIES))
clean: clean_targets clean_libraries

#all: build
re: clean build_libraries build_targets

#.PHONY: build_libraries build_targets build clean re
