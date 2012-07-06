AC_PREREQ([2.61])dnl Possibly earlier will do, but this is what I have
AC_INIT([payload_scraper], [0.0])
AC_CONFIG_SRCDIR([src/main.cc])
AC_LANG([C++])
AM_INIT_AUTOMAKE([foreign nostdinc silent-rules subdir-objects])
AM_MAINTAINER_MODE([enable])

### Programs ###

AC_PROG_CXX

### Libraries ###

### Language features ###

### Language features ###
AC_CHECK_HEADERS([execinfo.h],,,[/**/])

AX_CXX_DELETE_METHOD
AX_CXX_STATIC_ASSERT

### Output ###

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_COMMANDS_PRE([DEFS=])dnl Suppress pointless -DHAVE_CONFIG_H.
AC_OUTPUT
