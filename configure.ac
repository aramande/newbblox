#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([newbblox], [0.9], [aramande@gmail.com],  [newbblox])
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL

# Checks for arguments.
AC_ARG_ENABLE([debug],
              [  --enable-debug          Enable debug in NewbBlox (Warning, produces a -lot- of output)],
              debug=$enableval,
              debug="")

AC_ARG_ENABLE([warning],
              [  --enable-warning          Enable warnings in NewbBlox],
              warning=$enableval,
              warning="")

# Checks for libraries.
AC_CHECK_LIB([SDL_ttf], [TTF_OpenFont],,check_sdl_ttf="no")
AC_CHECK_LIB(SDL_image, IMG_Load,,check_sdl_image="no")

if test x${check_sdl_ttf} = xno; then
       AC_MSG_ERROR([ERROR: SDL_ttf library required])
fi
if test x${check_sdl_image} = xno; then
       AC_MSG_ERROR([ERROR: SDL_image library required])
fi
if test "x${debug}" = "xyes"; then
  AM_CXXFLAGS="$AM_CXXFLAGS -g -DDEBUG"
  AC_MSG_RESULT([Debug mode initiated])
fi

if test "x${warning}" = "xyes"; then
  AM_CXXFLAGS="$AM_CFLAGS -DWARN"
fi

# Setting flags
SDL_VERSION=1.2.13
AM_PATH_SDL($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)

CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

AM_CFLAGS="$AM_CFLAGS $SDL_CFLAGS" # g++ linker flags?
AM_LDFLAGS="$AM_LDFLAGS $SDL_LIBS" # or are they here?

DATA_FILES=`cd data ; find . \( -name CVS -o -name ".svn*" -o -name "Makefile*" -o -name wormux.desktop \) -prune -o -name ".#*" -o -type d -o -print`
DATA_FILES=`echo $DATA_FILES`


AC_SUBST([DATA_FILES])
AC_SUBST(AM_CXXFLAGS, "$AM_CXXFLAGS -DDATADIR=\"${datadir}\"") # g++ compilation flags go here
AC_SUBST(AM_LDFLAGS, "$AM_LDFLAGS")
AC_SUBST(PKGDATADIR, "${datadir}/${PACKAGE}")


# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 data/Makefile
                 src/Makefile])
AC_OUTPUT
