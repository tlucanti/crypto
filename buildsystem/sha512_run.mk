SHA3_512_BUILD_SRCS = sha512_run
SHA3_512_BUILD_TARGET = sha512
SHA3_512_BUILD_FLAGS = -I include
SHA3_512_BUILD_LIBRARIES = crypto

$(eval $(call build_executable,		\
	$(SHA3_512_BUILD_SRCS),			\
	$(SHA3_512_BUILD_TARGET),		\
	$(SHA3_512_BUILD_FLAGS),		\
	$(SHA3_512_BUILD_LIBRARIES)		\
))

$(eval $(call clean_target,			\
	$(SHA3_512_BUILD_SRCS),			\
	$(SHA3_512_BUILD_TARGET),		\
	$(SHA3_512_BUILD_FLAGS)			\
))
