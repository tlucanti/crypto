# $1 - src names
# $2 - taget name
# $3 - build flags


define build_executable

TARGETS += $(2)

${BUILD_DIR}/%.o: %.c
	$(CC) $(CFLAGS) $(3) -c $$^ -o $$@

$(addprefix __build_,$(2)): $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1)))
	$(CC) $(CFLAGS) \
		$(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1))) \
		-o $(addsuffix .elf,$(addprefix ${BUILD_DIR}/,$(2)))

$(2): $(addprefix __build_,$(2))

.PHONY: $(addprefix __build,$(2))
endef

define clean_target
$(addprefix __clean_,$(2)): Makefile
	rm -f $(addprefix ${BUILD_DIR}/,$(addsuffix .o,$(1)))
	rm -f $(addprefix ${BUILD_DIR}/,$(2))
endef

${BUILD_DIR}:
	mkdir -p ${BUILD_DIR}
