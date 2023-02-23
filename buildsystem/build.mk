# $1 - src dir
# $2 - src names
# $3 - taget name
# $4 - build flags
# $5 - libraries

W			=	\e[1;97m
G			=	\e[1;92m
Y			= 	\e[1;93m
R			=	\e[1;91m
C			=	\e[1;96m
P			=	\e[1;95m
S			=	\e[0m

#]]]]]]]

define build_executable

TARGETS += $(3)

${BUILD_DIR}/%.o: $(1)/%.c Makefile
	@printf "${G}[clang] ${W}$$<\n"
	@$(CC) $(CFLAGS) $(4) -c $$< -o $$@

$(addprefix __build_,$(3)): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2)))
	@printf "${P}[  ld ]${W}$(3).elf\n"
	@$(LD) $(LDFLAGS) -L${BUILD_DIR} \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2))) \
		$(addprefix -l,$(5)) \
		-o $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(3)))

$(3): $(addprefix __build_,$(3))

.PHONY: $(addprefix __build,$(3))
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
	@$(CC) $(CFLAGS) $(4) -c $$< -o $$@

$(addprefix ${BUILD_DIR}/,$(addsuffix .a,$(3))):
	@printf "${C}[ arc ]${W}$(3).a\n"
	@$(ARC) $(ARCFLAGS) \
		$(addsuffix .a,$(addprefix ${BUILD_DIR}/,$(3))) \
		$(addprefix -l,$(5)) \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2)))

$(addprefix __lib_,$(3)): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(2))) \
	$(addprefix ${BUILD_DIR}/,$(addsuffix .a,$(3)))

$(3): $(addprefix __lib_,$(3))

.PHONY: $(addprefix __lib_,$(3))
endef

define clean_library
$(addprefix __clean_lib_,$(3)):
	@printf "${R}[  rm ] ${W}$(addsuffix .a,$(3))\n"
	@${RM} $(addsuffix .a,$(addprefix ${BUILD_DIR}/,$(3)))
endef

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}
