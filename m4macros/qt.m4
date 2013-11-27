# Macros for checking Qt4 package
#
# $Id: gmp.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2013 Yusuke Matsunaga
# All rights reserved.


# ===================================================================
# YM_CHECK_QT([action-if-found], [action-if-not-found])
# ===================================================================
AC_DEFUN([YM_CHECK_QT],[dnl
AC_ARG_ENABLE([qt],
	      AS_HELP_STRING([--enable-qt], [Turn on QT]),
	      [
case "${enableval}" in
     yes) ym_tmp_qt=yes ;;
     no)  ym_tmp_qt=no ;;
     *) AC_MSG_ERROR([bad value ${enableval} for --enable-qt]) ;;
esac
],
[ym_tmp_qt=no])
AM_CONDITIONAL([WITH_QT], [test $ym_tmp_qt = yes])
if test $ym_tmp_qt = yes; then
  AT_WITH_QT
fi
])
