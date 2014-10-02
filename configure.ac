AC_PREREQ(2.59)

AC_INIT(README)

# versionning technique from Simple DirectMedia Layer (which they took from Gtk apparently)
#
# PROGRAM_MICRO_VERSION += 1	when bug fixes were done or small changes
# PROGRAM_MINOR_VERSION += 1	when new functions and or modules were added, anything more than mere bug fixes or small changes
# PROGRAM_MAJOR_VERSION += 1	when set goals for the whole projects are met, example when neuro will be done, we will raise that by 1. After it is 1, it only raise when most(like more than half) of the code was changes.
# if backward compatibility has been broken, put the 2 below to 0
# PROGRAM_INTERFACE_AGE += 1	if new functions were added put this to 0, else += 1
# PROGRAM_BINARY_AGE += 1		anything done adds 1
#

. version.mk

REJOY_MAJOR_VERSION=$MAJOR
REJOY_MINOR_VERSION=$MINOR
REJOY_MICRO_VERSION=$MICRO
REJOY_INTERFACE_AGE=$INTERFACE
REJOY_BINARY_AGE=$BINARY
REJOY_VERSION=$REJOY_MAJOR_VERSION.$REJOY_MINOR_VERSION.$REJOY_MICRO_VERSION

AC_SUBST(REJOY_MAJOR_VERSION)
AC_SUBST(REJOY_MINOR_VERSION)
AC_SUBST(REJOY_MICRO_VERSION)
AC_SUBST(REJOY_INTERFACE_AGE)
AC_SUBST(REJOY_BINARY_AGE)
AC_SUBST(REJOY_VERSION)


# libtool versioning
LT_RELEASE=$REJOY_MAJOR_VERSION.$REJOY_MINOR_VERSION
LT_CURRENT=`expr $REJOY_MICRO_VERSION - $REJOY_INTERFACE_AGE`
LT_REVISION=$REJOY_INTERFACE_AGE
LT_AGE=`expr $REJOY_BINARY_AGE - $REJOY_INTERFACE_AGE`

AC_SUBST(LT_RELEASE)
AC_SUBST(LT_CURRENT)
AC_SUBST(LT_REVISION)
AC_SUBST(LT_AGE)

AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([include/config.h])

AM_INIT_AUTOMAKE(recre, $REJOY_VERSION)

# call canonical host to get valuable informations about the current system
AC_CANONICAL_HOST
AC_DEFINE_UNQUOTED(OS, "${host}", [cpu-machine-OS])
AC_MSG_RESULT($host)

# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

# check for cygwin stuff
AC_LIBTOOL_WIN32_DLL

# libtool setup
AM_PROG_LIBTOOL


CFLAGS="$CFLAGS -std=iso9899:1990 -Wall -pedantic -I\$(top_srcdir)/include"

AC_ARG_ENABLE(Debug, [  --enable-Debug   	enable the debugging of this program [[default=no]]], , enable_Debug=no)

case $host in
	*-*-cygwin | *-*-mingw* | *-*-pw32*)
		targ=w32
		;;
	*)
		targ=unix
		;;
esac

# Checks for libraries.



# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([libintl.h locale.h stdlib.h string.h sys/time.h], 
		 , [AC_MSG_ERROR(You are missing important headers)])

case $targ in
	w32)
		AC_CHECK_HEADERS([windows.h], , [AC_MSG_ERROR(No default windows.h header found)])

		AC_CHECK_LIB(intl, gettext, , [AC_MSG_ERROR(You don't have libintl installed)])
	;;

	unix)
		AC_FUNC_REALLOC
		AC_CHECK_HEADERS([unistd.h], , [AC_MSG_ERROR(No default GNU/Linux header found)])
	;;
esac

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_STRUCT_TM

# Checks for library functions.
AC_CHECK_FUNCS([memset], 
	       , [AC_MSG_ERROR(You are missing important functions)])


dnl ----------------
dnl Debug toggle
dnl ----------------
if test x$enable_Debug = xyes; then
	        CFLAGS="$CFLAGS -g"
	else
		CFLAGS="$CFLAGS -O2 -s"
fi	

dnl ----------------
dnl gthread check
dnl ----------------
PKG_CHECK_MODULES(DEPS, gthread-2.0 >= 2.0)
CFLAGS="$CFLAGS $DEPS_CFLAGS"
LIBS="$LIBS $DEPS_LIBS"

dnl ----------------
dnl SDL check
dnl ----------------
if test $targ = unix; then
	AC_CHECK_LIB(SDL, SDL_Init, 
		     [AC_DEFINE(USE_SDL, 1, ["SDL main flag"]) LIBS="$LIBS `sdl-config --libs`" 
		     CFLAGS="$CFLAGS `sdl-config --cflags`"], 
		     [AC_MSG_ERROR(You don't have libSDL you can get it from http://www.libsdl.org)])

	AC_PATH_X
	AC_PATH_XTRA

	LIBS="$LIBS $X_LIBS"
	CFLAGS="$CFLAGS $X_CFLAGS"
	AC_CHECK_LIB(Xtst, XTestFakeKeyEvent, ,[AC_MSG_ERROR(You don't have libXtst)])

else
	AC_CHECK_LIB(SDL, SDL_Init, 
		     [AC_DEFINE(USE_SDL, 1, ["SDL main flag"]) LIBS="$LIBS -lSDL"], 
		     [AC_MSG_ERROR(You don't have libSDL you can get it from http://www.libsdl.org)])
fi

dnl ----------------
dnl GTK+ check
dnl ----------------
PKG_CHECK_MODULES(DEPS, gtk+-2.0 >= 2.2 glib-2.0 >= 2.2)
CFLAGS="$CFLAGS $DEPS_CFLAGS"
LIBS="$LIBS $DEPS_LIBS"




AC_SUBST(DEPS_CFLAGS)
AC_SUBST(DEPS_LIBS)


AC_CONFIG_FILES([
Makefile
include/Makefile
src/Makefile
])

AC_OUTPUT
