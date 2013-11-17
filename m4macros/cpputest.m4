# Macros for checking gmp package
#
# Copyright (C) 2013 Yusuke Matsunaga
# All rights reserved.

# ===================================================================
# YM_CHECK_CPPUTEST(action-if-found, action-if-not-found)
# if found, the following shell-variables are defined
# CPPUTEST_INCLUDES
# CPPUTEST_LIBS
#
# Also, the following C-macro is defined
# HAVE_CPPUTEST
# ===================================================================
# Checks for CppUTest package
AC_DEFUN([YM_CHECK_CPPUTEST],[dnl
AC_MSG_NOTICE([checking if CppUTest is available])
#
ym_tmp_lib_list="lib"
arch=`uname -p`
if test "X$arch" = "Xx86_64"; then
  ym_tmp_lib_list="lib64 $ym_tmp_lib_list"
fi
# --with-CppUTest-prefix
AC_ARG_WITH([CppUTest-prefix],
[  --with-CppUTest-prefix=DIR  CppUTestConfig.h is in $DIR/include/CppUTest],
[CppUTest_list="$withval"],
[CppUTest_list="$prefix $ac_default_prefix /usr /usr/local"])
#
# --with-CppUTest-libdir
AC_ARG_WITH([CppUTest-libdir],
[  --with-CppUTest-libdir=DIR  libCppUTest is in $DIR],
[CppUTest_libdir="$withval"],
[CppUTest_libdir=""])

CppUTest_found=0
CppUTest_prefix=""
CPPUTEST_INCLUDES=
CPPUTEST_LIBS=
for p in $CppUTest_list; do
  CppUTest_include=$p/include
  if test -f $CppUTest_include/CppUTest/CppUTestConfig.h; then
    CPPUTEST_INCLUDES="-I$CppUTest_include";
    CppUTest_found=1
    CppUTest_prefix=$p
    break
  fi
done

if test $CppUTest_found = 1; then
  CppUTest_found=0
  if test "x$CppUTest_libdir" = "x"; then
     for ym_tmp_lib in $ym_tmp_lib_list; do
        YM_CHECK_CPPUTESTLIB($CppUTest_prefix/$ym_tmp_lib)
	if test $CppUTest_found = 1; then
	   break
	fi
     done
  else
    YM_CHECK_CPPUTESTLIB($CppUTest_libdir)
  fi
fi
AS_IF([test $CppUTest_found = 1], [$1], [$2])
if test $CppUTest_found = 1; then
   AC_MSG_RESULT([CppUTest is found at $CPPUTEST_LIBS])
else
   AC_MSG_RESULT([CppUTest is not found, see http://cpputest.gitbuf.io/])
fi
])dnl

# ================================================================
# YM_CHECK_CPPUTESTLIB(libpath)
# ================================================================
# Checks for CppUTest libraries
AC_DEFUN([YM_CHECK_CPPUTESTLIB],[
  CppUTest_libdir=$1
  libs_old=$LIBS
  LIBS="$libs_old -L$CppUTest_libdir"
  AC_CHECK_LIB([CppUTest], [CHECK_EQUAL_C_INT_LOCATION],
       	       [CppUTest_found=1
		CPPUTEST_LIBS="-L$CppUTest_libdir -lCppUTest"
	      	AC_DEFINE([HAVE_CPPUTEST], 1, [Define if you have CppUTest package])
	        AC_SUBST([CPPUTEST_INCLUDES])
	        AC_SUBST([CPPUTEST_LIBS])
	        break
	       ], [],
	       [-lc])
  LIBS=$libs_old
])dnl
# end of CppUTest.m4
