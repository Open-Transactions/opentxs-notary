#  
#
AC_PREREQ([2.61])
AC_INIT([opentxs-server],m4_esyscmd(tr -d '\n' <VERSION), [ot-dev@opentransactions.org],[opentxs-server],[http://opentransactions.org])
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux/m4])

AM_INIT_AUTOMAKE([1.10 foreign])
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([no])])

## special prelims
AC_FUNC_STRNLEN

## libtool prelims
LT_PREREQ([2.2.4])
LT_INIT([dlopen])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O
AC_PROG_CPP
: ${CXXFLAGS=""}
: ${CFLAGS=""}
: ${CPPFLAGS=""}

PKG_PROG_PKG_CONFIG([0.25])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h float.h limits.h malloc.h memory.h netinet/in.h stddef.h stdint.h stdlib.h string.h sys/time.h sys/timeb.h unistd.h wchar.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_C_RESTRICT
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRERROR_R
AC_FUNC_STRTOD
AC_CHECK_FUNCS([getpass gettimeofday mkdir socket strtol strtoul])

# pthread #

AC_CHECK_LIB([pthread], [pthread_create])

## GCC AND CLANG DETECTION ##

AC_MSG_CHECKING([if compiling with clang])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[]], [[
#ifndef __clang__
	not clang
#endif
	]])],[ac_cv_vp_clang=true],[ac_cv_vp_clang=false])

AC_MSG_RESULT([$ac_cv_vp_clang])

if test x"$ac_cv_vp_clang" == x"true"; then
	AC_MSG_CHECKING([if compiling with clang -std=c++11])
	AC_LANG_PUSH([C++])
	AC_COMPILE_IFELSE(
		[AC_LANG_PROGRAM([[#include <cinttypes>]],
			[[]])],
		[ac_cv_vp_clang11=true],
		[ac_cv_vp_clang11=false])
	AC_LANG_POP
	AC_MSG_RESULT([$ac_cv_vp_clang11])
fi

if test x"$ac_cv_vp_clang" == x"true"; then
	AC_MSG_CHECKING([if compiling with clang TR1])
	AC_LANG_PUSH([C++])
	   AC_COMPILE_IFELSE(
			 [AC_LANG_PROGRAM([[#include <tr1/memory>]],
				    [[]])],
			 [ac_cv_vp_clangtr1=true],
			[ac_cv_vp_clangtr1=false])
	AC_LANG_POP
	AC_MSG_RESULT([$ac_cv_vp_clangtr1])
fi

if test x"$ac_cv_vp_clang" == x"false"; then
	AC_MSG_CHECKING([whether gcc version is sufficient])
	gcc_major=0
	gcc_minor=0

	GCC_VERSION=`$CC -dumpversion`
	if test $? -eq 0; then
		gcc_major=`echo $GCC_VERSION | cut -d. -f1`
		gcc_minor=`echo $GCC_VERSION | cut -d. -f2`
	fi

	## test if we have a version of gcc that suports c++11
	ac_cv_vp_gcc11=false

	if test $gcc_major -ge 4 -a $gcc_minor -ge 7 ; then
		ac_cv_vp_gcc11=true
		AC_MSG_RESULT([yes, and supports c++11])
	elif test $gcc_major -ge 4 -a $gcc_minor -ge 2 ; then
		ac_cv_vp_gcc11=false
		AC_MSG_RESULT([yes, but no c++11 support])
	else
		AC_MSG_ERROR([If using GCC, version 4.2.0 or later is required.])
		AC_MSG_RESULT([no])
	fi
fi

if test x"$ac_cv_vp_clang" == x"true" -a x"$ac_cv_vp_clang11" == x"false"; then
	ac_cv_vp_cxx11=false
elif test x"$ac_cv_vp_clang" == x"false" -a x"$ac_cv_vp_gcc11" == x"false"; then
	ac_cv_vp_cxx11=false
else
	ac_cv_vp_cxx11=true
fi

if test x"$ac_cv_vp_cxx11" == x"true"; then
	AC_SUBST([CXX11_FLAGS],["-std=c++11"])
else
	AC_DEFINE([CXX03_TR1], [1], [Building with C++03 TR1])
fi

######## Prelims end, Custom checks begin #######

##### Dependencies, packages, libs ####

#
#
#
PKG_CHECK_MODULES([OPENTXS],[opentxs >= 0.0.1],,
  AC_MSG_ERROR([Error: libotapi version >= 0.0.1 not found!])
)

##############################
## Build Configuration Options


## Keyring Options ##

AC_ARG_WITH(	[keyring],
		[AS_HELP_STRING([--with-keyring@<:@=ARG@:>@],
		[Choose one keyring ARG from: windows, mac, gnome, kwallet, flatfile])],
		[case "${withval}" in
##			windows) true ;;
			mac) true ;;
			gnome) true ;;
			kwallet) true ;;
			flatfile) true ;;
			*) AC_MSG_ERROR([bad value ${withval} for --with-keyring]) ;;
			esac],
		[])

AM_CONDITIONAL([KEYRING_WINDOWS],	[test x"$with_keyring" == x"windows"])
AM_CONDITIONAL([KEYRING_MAC],		[test x"$with_keyring" == x"mac"])
AM_CONDITIONAL([KEYRING_GNOME],		[test x"$with_keyring" == x"gnome"])
AM_CONDITIONAL([KEYRING_KWALLET],	[test x"$with_keyring" == x"kwallet"])
AM_CONDITIONAL([KEYRING_FLATFILE],  [test x"$with_keyring" == x"flatfile"])

AS_IF(	[test x"$with_keyring" == x"windows"],
	[
	AC_SUBST(KRG_CFLAGS,[""])
	AC_SUBST(KRG_LIBS,[""])
	AC_DEFINE([KEYRING_WINDOWS], [1], [Using Windows Keyring])
	])
##  WINDOWS build flags needed here #

AS_IF(	[test x"$with_keyring" == x"mac"],
	[
	AC_SUBST(KRG_CFLAGS,["-isysroot /Developer/SDKs/MacOSX10.6.sdk -arch ppc -arch i386"])
	AC_SUBST(KRG_LIBS,["-syslibroot /Developer/SDKs/MacOSX10.6.sdk -arch ppc -arch i386"])
	AC_DEFINE([KEYRING_MAC], [1], [Using Mac Keyring])
	])

AS_IF(	[test x"$with_keyring" == x"gnome"],
	[
	PKG_CHECK_MODULES([KRG],[glib-2.0 >= 2.28.6 gnome-keyring-1])
	AC_DEFINE([KEYRING_GNOME], [1], [Using Gnome Keyring])
	])

AS_IF(	[test "$with_keyring" == x"kwallet"],
	[
	AC_SUBST(KRG_CFLAGS,	["-I/usr/include/kde"])
	AC_SUBST(KRG_LIBS,	["-L/usr/lib -lkwalletclient"])
	AC_DEFINE([KEYRING_KWALLET], [1], [Using KWallet Keyring])
	])
	
AS_IF(  [test "$with_keyring" == x"flatfile"],
    [
    AC_SUBST(KRG_CFLAGS,  [""])
    AC_SUBST(KRG_LIBS,  [""])
    AC_DEFINE([KEYRING_FLATFILE], [1], [Using flatfile Keyring])
    ])

AS_IF(  [test "$with_keyring" == x"flatfile"],
    [
    AC_SUBST(KRG_CFLAGS,  [""])
    AC_SUBST(KRG_LIBS,  [""])
    AC_DEFINE([KEYRING_FLATFILE], [1], [Using flatfile Keyring])
    ])



## (Option) Signal Handler ##

AC_ARG_ENABLE(
	sighandler,
	AS_HELP_STRING(
		[--enable-sighandler],
		[enable sighandler, default: no]),
	[case "${enableval}" in
		yes)	sighandler=true ;;
		no)	sighandler=false ;;
		*)	AC_MSG_ERROR([bad value ${enableval} for --disable-sighandler]) ;;
		esac],
	[sighandler=false])

AM_CONDITIONAL(ENABLE_OT_SIGNAL_HANDLING, test x"$sighandler" == x"true")

if test x"$sighandler" == x"true"; then
	AC_DEFINE([OT_SIGNAL_HANDLING], [1], [Using OT Signal Handling])
fi

## (Option) Warnings ##

AC_ARG_ENABLE(
	warnings,
	AS_HELP_STRING(
		[--enable-warnings],
		[enable warnings, default: no]),
	[case "${enableval}" in
		yes) warnings=true ;;
		no)  warnings=false ;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-warnings]) ;;
		esac],
	[warnings=false])

AM_CONDITIONAL(ENABLE_WARNINGS, test x"$warnings" == x"true")

if test x"$warnings" == x"true"; then

	AC_SUBST([WARN_CXXFLAGS],
	["-Wall -Waddress -Woverloaded-virtual -Wshadow -Wconversion"])
fi

## (Option) Build In Release Mode ##


AC_ARG_ENABLE([debug],
	AS_HELP_STRING([--enable-debug],[enable debug, default: no]),
	[case "${enableval}" in
		yes) debug=true ;;
		no)  debug=false ;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
		esac],[debug=false])
AM_CONDITIONAL([ENABLE_DEBUG], [test x$debug == x"true"])


AC_ARG_ENABLE([release],
	AS_HELP_STRING([--enable-release],[enable release, default: no]),
	[case "${enableval}" in
		yes) release=true ;;
		no)  release=false ;;
		*)   AC_MSG_ERROR([bad value ${enableval} for --enable-release]) ;;
		esac],[release=false])
AM_CONDITIONAL([ENABLE_RELEASE], [test x$release == x"true"])

if test x"$debug" == x"true" -a x"$release" == x"true"; then
	AC_MSG_ERROR([You must not set both release and debug modes! (remove either: --enable-debug or --enable-release).])
fi

if test x"$debug" == x"false" -a x"$release" == x"false"; then
	AC_MSG_RESULT([defaulting to RELEASE mode!])
	release=true
	AM_CONDITIONAL([ENABLE_RELEASE], [test x$release == x"true"])
fi

if test x"$debug" == x"true"; then
	if test x"$ac_cv_vp_clang" == x"true"; then
		AC_SUBST([DBG_CXXFLAGS],["-g3 -O0 -fPIC"])
	else
		AC_SUBST([DBG_CXXFLAGS],["-g3 -O0 -fPIC -DPIC"])
	fi
	AC_DEFINE([DEBUG], [1], [Building in Debug Mode])
fi

if test x"$release" == x"true"; then
	if test x"$ac_cv_vp_clang" == x"true"; then
		AC_SUBST([REL_CXXFLAGS],["-g0 -O3 -fPIC -D_FORTIFY_SOURCE=2"])
	else
		AC_SUBST([REL_CXXFLAGS],["-g0 -O0 -fPIC -D_FORTIFY_SOURCE=2 --param ssp-buffer-size=4 -fstack-protector-all -fstack-check -Wstack-protector"])
	fi
	AC_DEFINE([RELEASE], [1], [Building in Release Mode])
fi

#####   Output to AM   #####

AC_SUBST([DEPS_CFLAGS], ["$OPENTXS_CFLAGS"])

AC_SUBST([DEPS_LIBS],	  ["$OPENTXS_LIBS"])

AC_SUBST([AM_CXXFLAGS],	["$DBG_CXXFLAGS $REL_CXXFLAGS $WARN_CXXFLAGS $CXX11_FLAGS $DEPS_CFLAGS"])

AC_CONFIG_FILES([Makefile
                 ])

AC_OUTPUT