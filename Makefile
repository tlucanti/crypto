
CFLAGS		=	-Wall -Wextra -D __debug -O0 -g3
CC			=	clang

BUILD_ROOT	=	buildsystem
BUILD_DIR	=	build

TARGETS		=

include ${BUILD_ROOT}/build.mk

include ${BUILD_ROOT}/md5_run.mk
include ${BUILD_ROOT}/sha224_run.mk
include ${BUILD_ROOT}/sha256_run.mk
include ${BUILD_ROOT}/sha384_run.mk
include ${BUILD_ROOT}/sha512_run.mk

build_targets: ${BUILD_DIR} $(addprefix __build_,$(TARGETS))

clean: $(addprefix __clean_,$(TARGETS))

re: clean build_targets

