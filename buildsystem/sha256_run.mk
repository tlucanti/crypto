SHA3_256_BUILD_SRCS = sha256_run
SHA3_256_BUILD_TARGET = sha256
SHA3_256_BUILD_FLAGS = -I include
SHA3_256_BUILD_LIBRARIES = crypto

$(eval $(call build_executable,		\
	$(SHA3_256_BUILD_SRCS),			\
	$(SHA3_256_BUILD_TARGET),		\
	$(SHA3_256_BUILD_FLAGS),		\
	$(SHA3_256_BUILD_LIBRARIES)		\
))

$(eval $(call clean_target,			\
	$(SHA3_256_BUILD_SRCS),			\
	$(SHA3_256_BUILD_TARGET),		\
	$(SHA3_256_BUILD_FLAGS)			\
))
