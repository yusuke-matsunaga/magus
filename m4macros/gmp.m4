# Macros for checking gmp package
#
# $Id: gmp.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2009 Yusuke Matsunaga
# All rights reserved.

# ===================================================================
# YM_CHECK_GMP
# if found, the following shell-variables are defined
# GMP_INCLUDES
# GMP_LIBS
#
# Also, the following C-macro is defined
# HAVE_GMP
# ===================================================================
# Checks for gmp package
AC_DEFUN([YM_CHECK_GMP],[dnl
AC_MSG_NOTICE([checking if gmp is available])
#
ym_tmp_lib_list="lib"
arch=`uname -p`
if test "X$arch" = "Xx86_64"; then
  ym_tmp_lib_list="lib64 $ym_tmp_lib_list"
fi
# --with-gmp-prefix
AC_ARG_WITH([gmp-prefix],
[  --with-gmp-prefix=DIR  gmpxx.h is in $DIR/include],
[gmp_list="$withval"],
[gmp_list="$prefix $ac_default_prefix /usr /usr/local"])
#
# --with-gmp-libdir
AC_ARG_WITH([gmp-libdir],
[  --with-gmp-libdir=DIR  libgmp is in $DIR],
[gmp_libdir="$withval"],
[gmp_libdir=""])

gmp_found=0
gmp_prefix=""
GMP_INCLUDES=
GMP_LIBS=
for p in $gmp_list; do
  gmp_include=$p/include
  if test -f $gmp_include/gmpxx.h; then
    GMP_INCLUDES="-I$gmp_include";
    gmp_found=1
    gmp_prefix=$p
    break
  fi
done

if test $gmp_found = 1; then
  gmp_found=0
  if test "x$gmp_libdir" = "x"; then
     for ym_tmp_lib in $ym_tmp_lib_list; do
        YM_CHECK_GMPLIB($gmp_prefix/$ym_tmp_lib)
     done
  else
    YM_CHECK_GMPLIB($gmp_libdir)
  fi
fi
if test $gmp_found = 1; then
   AC_MSG_RESULT([gmp is found at $GMP_LIBS])
else
   AC_MSG_RESULT([gmp is not found, see http://gmplib.org/])
fi
])dnl

# ================================================================
# YM_CHECK_GMPLIB(libpath)
# ================================================================
# Checks for gmp libraries
AC_DEFUN([YM_CHECK_GMPLIB],[
  gmp_libdir=$1
  libs_old=$LIBS
  LIBS="$libs_old -L$gmp_libdir"
  AC_CHECK_LIB([gmp], [__gmpz_init],
       	       [gmp_found=1
		GMP_LIBS="-L$gmp_libdir -lgmp -lgmpxx"
	      	AC_DEFINE([HAVE_GMP], 1, [Define if you have gmp package])
	        AC_SUBST([GMP_INCLUDES])
	        AC_SUBST([GMP_LIBS])
	        break
	       ], [],
	       [-lc])
  LIBS=$libs_old
])dnl
# end of gmp.m4
