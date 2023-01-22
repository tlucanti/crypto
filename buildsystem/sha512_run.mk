SHA3_512_BUILD_SRCS = sha512_run sha3
SHA3_512_BUILD_TARGET = sha512
SHA3_512_BUILD_FLAGS = -I include

$(eval $(call build_executable,		\
	$(SHA3_512_BUILD_SRCS),			\
	$(SHA3_512_BUILD_TARGET),		\
	$(SHA3_512_BUILD_FLAGS)			\
))

$(eval $(call clean_target,			\
	$(SHA3_512_BUILD_SRCS),			\
	$(SHA3_512_BUILD_TARGET),		\
	$(SHA3_512_BUILD_FLAGS)			\
))
