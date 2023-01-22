SHA3_384_BUILD_SRCS = sha384_run sha3
SHA3_384_BUILD_TARGET = sha384
SHA3_384_BUILD_FLAGS = -I include

$(eval $(call build_executable,		\
	$(SHA3_384_BUILD_SRCS),			\
	$(SHA3_384_BUILD_TARGET),		\
	$(SHA3_384_BUILD_FLAGS)			\
))

$(eval $(call clean_target,			\
	$(SHA3_384_BUILD_SRCS),			\
	$(SHA3_384_BUILD_TARGET),		\
	$(SHA3_384_BUILD_FLAGS)			\
))
