SHA3_224_BUILD_SRCS = sha224_run
SHA3_224_BUILD_TARGET = sha224
SHA3_224_BUILD_FLAGS = -I include
SHA3_224_BUILD_LIBRARIES = crypto

$(eval $(call build_executable,		\
	$(SHA3_224_BUILD_SRCS),			\
	$(SHA3_224_BUILD_TARGET),		\
	$(SHA3_224_BUILD_FLAGS),		\
	$(SHA3_224_BUILD_LIBRARIES)		\
))

$(eval $(call clean_target,			\
	$(SHA3_224_BUILD_SRCS),			\
	$(SHA3_224_BUILD_TARGET),		\
	$(SHA3_224_BUILD_FLAGS)			\
))
