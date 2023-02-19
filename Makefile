
CFLAGS		=	-Wall -Wextra -Wmost -Wpedantic
CFLAGE		+=	-D__debug -O0 -g3
CC			=	clang
LD			=	clang
RM			=	rm -f

BUILD_ROOT	=	buildsystem
BUILD_DIR	=	build

TARGETS		=

CFLAGS 		+= -Wall -Wextra -Warray-bounds-pointer-arithmetic \
			   -Warray-parameter -Wassign-enum -Watomic-implicit-seq-cst \
			   -Wbad-function-cast -Wbitfield-enum-conversion \
			   -Wbitwise-instead-of-logical -Wbitwise-op-parentheses \
			   -Wbool-operation -Wcalled-once-parameter -Wcast-align \
			   -Wcast-function-type -Wcast-qual \
			   -Wchar-align -Wchar-subscripts -Wcomma -Wcomment \
			   -Wcompletion-handler -Wcompound-token-split-by-space \
			   -Wconditional-uninitialized -Wconsumed -Wconversion \
			   -Wcovered-switch-default -Wcustom-atomic-properties -Wdate-time \
			   -Wdealloc-in-category -Wdeclaration-after-statement \
			   -Wdelimited-escape-sequence-extension -Wdeprecated \
			   -Wdirect-ivar-access -Wdisabled-macro-expansion \
			   -Wdollar-in-identifier-extension -Wdouble-promotion \
			   -Wduplicate-decl-specifier -Wduplicate-enum \
			   -Wduplicate-method-arg -Wduplicate-method-match \
			   -Wembedded-directive -Wempty-init-stmt -Wempty-translation-unit \
			   -Wenum-compare-conditional -Wenum-enum-conversion \
			   -Wenum-float-conversion -Wexit-time-destructors \
			   -Wexpansion-to-defined -Wextra-semi-stmt -Wfloat-conversion \
			   -Wfloat-equal -Wfloat-overflow-conversion \
			   -Wfloat-zero-conversion -Wfor-loop-analysis -Wformat-non-iso \
			   -Wformat-nonliteral -Wformat-pedantic -Wformat-type-confusion \
			   -Wfour-char-constants -Wframe-address -Widiomatic-parentheses \
			   -Wignored-qualifiers -Wimplicit -Wimplicit-atomic-properties \
			   -Wimplicit-fallthrough -Wimplicit-float-conversion \
			   -Wimplicit-function-declaration -Wimplicit-int \
			   -Wimplicit-int-conversion -Wimplicit-int-float-conversion \
			   -Wimplicit-retain-self -Wimport-preprocessor-directive-pedantic \
			   -Wincompatible-function-pointer-types \
			   -Wincomplete-module -Winfinite-recursion -Wint-in-bool-context \
			   -Wkeyword-macro -Wlogical-op-parentheses -Wmain -Wmicrosoft \
			   -Wmisleading-indentation -Wmissing-braces \
			   -Wmissing-field-initializers -Wmissing-noreturn \
			   -Wmissing-prototypes -Wmissing-variable-declarations \
			   -Wnewline-eof -Wnull-pointer-arithmetic \
			   -Wnull-pointer-subtraction -Wover-aligned -Woverlength-strings \
			   -Wpacked -Wpadded -Wparentheses -Wpointer-arith \
			   -Wprofile-instr-missing -Wredundant-parens \
			   -Wshift-sign-overflow -Wshorten-64-to-32 -Wsign-compare \
			   -Wsign-conversion -Wsometimes-uninitialized -Wstatic-in-inline \
			   -Wstrict-prototypes -Wstrict-selector-match \
			   -Wstring-concatenation -Wstring-conversion -Wsuspicious-bzero \
			   -Wswitch-enum -Wtautological-bitwise-compare \
			   -Wtautological-compare -Wtautological-overlap-compare\
			   -Wtautological-type-limit-compare \
			   -Wtautological-unsigned-char-zero-compare \
			   -Wtautological-unsigned-enum-zero-compare \
			   -Wtautological-unsigned-zero-compare \
			   -Wtautological-value-range-compare -Wthread-safety \
			   -Wthread-safety-beta -Wthread-safety-negative \
			   -Wthread-safety-verbose -Wtrigraphs -Wtype-limits \
			   -Wunaligned-access -Wundeclared-selector -Wundef -Wundef-prefix \
			   -Wunneeded-internal-declaration -Wunreachable-code \
			   -Wunreachable-code-aggressive -Wunused \
			   -Wunused-but-set-parameter -Wunused-but-set-variable \
			   -Wunused-const-variable -Wunused-volatile-lvalue \
			   -Wused-but-marked-unused -Wvector-conversion -Wvla-extension \
			   -Wzero-length-array

include ${BUILD_ROOT}/build.mk

include ${BUILD_ROOT}/md5_run.mk
include ${BUILD_ROOT}/sha224_run.mk
include ${BUILD_ROOT}/sha256_run.mk
include ${BUILD_ROOT}/sha384_run.mk
include ${BUILD_ROOT}/sha512_run.mk

build_targets: ${BUILD_DIR} $(addprefix __build_,$(TARGETS))

clean: $(addprefix __clean_,$(TARGETS))

re: clean build_targets

