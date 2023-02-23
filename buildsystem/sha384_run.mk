SHA3_384_BUILD_DIR = src
SHA3_384_BUILD_SRCS = sha384_run
SHA3_384_BUILD_TARGET = sha384
SHA3_384_BUILD_FLAGS = -I include
SHA3_384_BUILD_LIBRARIES = crypto

$(eval $(call build_executable,		\
	$(SHA3_384_BUILD_DIR),			\
	$(SHA3_384_BUILD_SRCS),			\
	$(SHA3_384_BUILD_TARGET),		\
	$(SHA3_384_BUILD_FLAGS),		\
	$(SHA3_384_BUILD_LIBRARIES)		\
))

$(eval $(call clean_target,			\
	$(SHA3_384_BUILD_DIR),			\
	$(SHA3_384_BUILD_SRCS),			\
	$(SHA3_384_BUILD_TARGET),		\
	$(SHA3_384_BUILD_FLAGS)			\
))
