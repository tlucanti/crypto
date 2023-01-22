SHA3_224_BUILD_SRCS = sha224_run sha3
SHA3_224_BUILD_TARGET = sha224
SHA3_224_BUILD_FLAGS = -I include

$(eval $(call build_executable,		\
	$(SHA3_224_BUILD_SRCS),			\
	$(SHA3_224_BUILD_TARGET),		\
	$(SHA3_224_BUILD_FLAGS)			\
))

$(eval $(call clean_target,			\
	$(SHA3_224_BUILD_SRCS),			\
	$(SHA3_224_BUILD_TARGET),		\
	$(SHA3_224_BUILD_FLAGS)			\
))
