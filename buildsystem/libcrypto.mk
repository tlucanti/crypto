LIB_CRYPTO_BUILD_SRCS = md5 sha3
LIB_CRYPTO_BUILD_TARGET = libcrypto
LIB_CRYPTO_BUILD_FLAGS = -I include

$(eval $(call build_library,		\
	$(LIB_CRYPTO_BUILD_SRCS),		\
	$(LIB_CRYPTO_BUILD_TARGET),		\
	$(LIB_CRYPTO_BUILD_FLAGS)		\
))

$(eval $(call clean_library,		\
	$(LIB_CRYPTO_BUILD_SRCS),		\
	$(LIB_CRYPTO_BUILD_TARGET),		\
	$(LIB_CRYPTO_BUILD_FLAGS)		\
))
