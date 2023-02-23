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

${BUILD_DIR}/%.o: %.c Makefile
	@printf "${G}[clang] ${W}$$<\n"
	@$(CC) $(CFLAGS) $(3) -c $$< -o $$@

$(addprefix __build_,$(2)): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1)))
	@printf "${P}[  ld ]${W}$(2).elf\n"
	@$(LD) $(LDFLAGS) -L${BUILD_DIR} \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1))) \
		$(addprefix -l,$(4)) \
		-o $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(2)))

$(2): $(addprefix __build_,$(2))

.PHONY: $(addprefix __build,$(2))
endef

define clean_target
$(addprefix __clean_,$(2)):
	@printf "${R}[  rm ] ${W}$(addsuffix .o,$(1))\n"
	@printf "${R}[  rm ] ${W}$(addsuffix .elf,$(2))\n"
	@${RM} $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1)))
	@${RM} $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(2)))
endef

define build_library

LIBRARIES += $(2)

${BUILD_DIR}/%.o: %.c Makefile
	@printf "${G}[clang] ${W}$$<\n"
	@$(CC) $(CFLAGS) $(3) -c $$< -o $$@

$(addprefix ${BUILD_DIR}/,$(addsuffix .a,$(2))):
	@printf "${C}[ arc ]${W}${2}.a\n"
	@$(ARC) $(ARCFLAGS) \
		$(addsuffix .a,$(addprefix ${BUILD_DIR}/,$(2))) \
		$(addprefix -l,$(4)) \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1)))

$(addprefix __lib_,$(2)): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1))) \
	$(addprefix ${BUILD_DIR}/,$(addsuffix .a,$(2)))

$(2): $(addprefix __lib_,$(2))

.PHONY: $(addprefix __lib_,$(2))
endef

define clean_library
$(addprefix __clean_lib_,$(2)):
	@printf "${R}[  rm ] ${W}$(addsuffix .a,$(2))\n"
	@${RM} $(addsuffix .a,$(addprefix ${BUILD_DIR}/,$(2)))
endef

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}
