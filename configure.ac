AC_PREREQ([2.61])dnl Possibly earlier will do, but this is what I have
AC_INIT([payload_scraper], [0.0])
AC_CONFIG_SRCDIR([src/main.cc])
AC_LANG([C++])
AM_INIT_AUTOMAKE([foreign nostdinc silent-rules subdir-objects])
AM_MAINTAINER_MODE([enable])

### Programs ###

AC_PROG_CXX
AC_PROG_RANLIB
PKG_PROG_PKG_CONFIG


### Libraries ###
# Presently no need for libssl, only libcrypto.
# We require version 1.0.1 for GCM support.
PKG_CHECK_MODULES([libcrypto], [libcrypto >= 1.0.1])
# libevent 2.0 radically changed the API
PKG_CHECK_MODULES([libz], [zlib >= 1.2.3.4])

LIBS="$libevent_LIBS $libcrypto_LIBS $libz_LIBS -lboost_system -lboost_filesystem -lcurl"


### Language features ###

### Language features ###
AC_CHECK_HEADERS([execinfo.h],,,[/**/])

#AX_CXXFLAGS_STDCXX_11([ext])
#AX_CXX_DELETE_METHOD
#AX_CXX_STATIC_ASSERT

### Output ###

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_COMMANDS_PRE([DEFS=])dnl Suppress pointless -DHAVE_CONFIG_H.
AC_OUTPUT
