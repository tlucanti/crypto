# $1 - src names
# $2 - taget name
# $3 - build flags

W			=	\e[1;97m
G			=	\e[1;92m
Y			= 	\e[1;93m
R			=	\e[1;91m
C			=	\e[1;96m
P			=	\e[1;95m
S			=	\e[0m

#]]]]]]]

define build_executable

TARGETS += $(2)

${BUILD_DIR}/%.o: %.c
	@printf "${G}[clang] ${W}$$^\n"
	@$(CC) $(CFLAGS) $(3) -c $$^ -o $$@

$(addprefix __build_,$(2)): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1)))
	@printf "${P}[  ld ]${W}$(2).elf\n"
	@$(LD) $(CFLAGS) \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1))) \
		-o $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(2)))

$(2): $(addprefix __build_,$(2))

.PHONY: $(addprefix __build,$(2))
endef

define clean_target
$(addprefix __clean_,$(2)): Makefile
	@printf "${R}[  rm ]${W}$(1)\n"
	@printf "${R}[  rm ]${W}$(2)\n"
	@${RM} $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1)))
	@${RM} -f $(addprefix ${BUILD_DIR}/,$(2))
endef

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}
