SHA3_256_BUILD_SRCS = sha256_run sha3
SHA3_256_BUILD_TARGET = sha256
SHA3_256_BUILD_FLAGS = -I include

$(eval $(call build_executable,		\
	$(SHA3_256_BUILD_SRCS),			\
	$(SHA3_256_BUILD_TARGET),		\
	$(SHA3_256_BUILD_FLAGS)			\
))

$(eval $(call clean_target,			\
	$(SHA3_256_BUILD_SRCS),			\
	$(SHA3_256_BUILD_TARGET),		\
	$(SHA3_256_BUILD_FLAGS)			\
))
