MD5_BUILD_SRCS = md5_run md5
MD5_BUILD_TARGET = md5
MD5_BUILD_FLAGS = -I include

$(eval $(call build_executable,		\
	$(MD5_BUILD_SRCS),				\
	$(MD5_BUILD_TARGET),			\
	$(MD5_BUILD_FLAGS)				\
))

$(eval $(call clean_target,			\
	$(MD5_BUILD_SRCS),				\
	$(MD5_BUILD_TARGET),			\
	$(MD5_BUILD_FLAGS)				\
))
