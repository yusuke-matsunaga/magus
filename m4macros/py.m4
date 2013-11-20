# Macros for checking Python package
#
# $Id: popt.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2010 Yusuke Matsunaga
# All rights reserved.

# ===================================================================
# YM_CHECK_PYTHON(action-if-found, action-if-not-found)
# if found, the following shell-variables are defined
# PYTHON_CPPFLAGS
# PYTHON_LDFLAGS
#
# Also, the following C-macro is defined
# HAVE_PYTHON
# ===================================================================
# Checks for Python package
AC_DEFUN([YM_CHECK_PYTHON],[dnl
#
# --with-python-cflags
AC_ARG_WITH([python-cflags],
[AS_HELP_STRING([--with-python-cflags=CFLAGS],
		[Specify CFLAGS for Python])],
[py_cflags="$withval"],
[dnl
  if test -x `which python-config`; then
    py_cflags=`python-config --cflags`
  else
    py_cflags=""
  fi
])
# --with-python-ldflags
AC_ARG_WITH([python-ldflags],
[AS_HELP_STRING([--with-python-ldflags=LDFLAGS],
		[Specify LDFLAGS for Python])],
[py_ldflags="$withval"],
[dnl
  if test -x `which python-config`; then
    py_ldflags=`python-config --ldflags`
  else
    py_ldflags=""
  fi
])
#
old_cppflags=$CPPFLAGS
old_cflags=$CFLAGS
old_cxxflags=$CXXFLAGS
CPPFLAGS="$py_cflags $CPPFLAGS"
CFLAGS="$py_cflags $CFLAGS"
CXXFLAGS="$py_cflags $CXXFLAGS"
AC_CHECK_HEADER([Python.h],
[dnl
  AC_DEFINE([HAVE_PYTHON], 1, [Define if you have Python headers])
  PYTHON_CPPFLAGS=$py_cflags
  PYTHON_LDFLAGS=$py_ldflags
  AC_SUBST([PYTHON_CPPFLAGS])
  AC_SUBST([PYTHON_LDFLAGS])
  py_found=1
], [py_found=0])
CPPFLAGS=$old_cppflags
CFLAGS=$old_cflags
CXXFLAGS=$old_cflags
AM_CONDITIONAL([WITH_PYTHON], [test $py_found = 1])
AS_IF([test $py_found = 1], [$1], [$2])
])dnl
# end of py.m4
