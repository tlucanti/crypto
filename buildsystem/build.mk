# $1 - src dir
# $2 - src names
# $3 - taget name
# $4 - build flags
# $5 - libraries

K			=	\e[1;30m
R			=	\e[1;31m
G			=	\e[1;32m
Y			=	\e[1;33m
B			=	\e[1;34m
P			=	\e[1;35m
C			=	\e[1;36m
W			=	\e[1;37m

#]]]]]]]]

define build_executable

TARGETS += $(3)

${BUILD_DIR}/%.o: $(1)/%.c Makefile
	@printf "${G}[clang] ${W}$$<\n"
	@$(CC) $(CFLAGS) $(COPTIONS) $(4) -c $$< -o $$@

$(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(3))): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2)))
	@printf "${P}[  ld ]${W}$(3).elf\n"
	@$(LD) $(LDFLAGS) -L${BUILD_DIR} \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2))) \
		$(addprefix -l,$(5)) \
		-o $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(3)))

$(3): $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(3)))
endef

define clean_target
$(addprefix __clean_,$(3)):
	@printf "${R}[  rm ] ${W}$(addsuffix .o,$(2))\n"
	@printf "${R}[  rm ] ${W}$(addsuffix .elf,$(3))\n"
	@${RM} $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2)))
	@${RM} $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(3)))
endef

define build_library

LIBRARIES += $(3)

${BUILD_DIR}/%.o: %.c Makefile
	@printf "${G}[clang] ${W}$$<\n"
	@$(CC) $(CFLAGS) $(COPTIONS) $(4) -c $$< -o $$@

$(addprefix ${BUILD_DIR}/,$(addsuffix .a,$(3))):
	@printf "${C}[ arc ]${W}$(3).a\n"
	@$(ARC) $(ARCFLAGS) \
		$(addsuffix .a,$(addprefix ${BUILD_DIR}/,$(3))) \
		$(addprefix -l,$(5)) \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2)))

$(addprefix __lib_,$(3)): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2))) \
	$(addprefix ${BUILD_DIR}/,$(addsuffix .a,$(3)))

$(3): $(addprefix __lib_,$(3))
endef

define clean_library
$(addprefix __clean_lib_,$(3)):
	@printf "${R}[  rm ] ${W}$(addsuffix .o,$(2))\n"
	@printf "${R}[  rm ] ${W}$(addsuffix .a,$(3))\n"
	@${RM} $(addsuffix .o,$(addprefix ${BUILD_DIR}/,$(2)))
	@${RM} $(addsuffix .a,$(addprefix ${BUILD_DIR}/,$(3)))
endef

${BUILD_DIR}:
	@printf "${B}[mkdir] ${W}${BUILD_DIR}\n"
	@mkdir -p ${BUILD_DIR}
