MD5_BUILD_SRCS = md5_run
MD5_BUILD_TARGET = md5
MD5_BUILD_FLAGS = -I include
MD5_BUILD_LIBRARIES = crypto

$(eval $(call build_executable,		\
	$(MD5_BUILD_SRCS),				\
	$(MD5_BUILD_TARGET),			\
	$(MD5_BUILD_FLAGS),				\
	$(MD5_BUILD_LIBRARIES)			\
))

$(eval $(call clean_target,			\
	$(MD5_BUILD_SRCS),				\
	$(MD5_BUILD_TARGET),			\
	$(MD5_BUILD_FLAGS)				\
))
