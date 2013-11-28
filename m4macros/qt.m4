# Macros for checking Qt4 package
#
# $Id: gmp.m4 2507 2009-10-17 16:24:02Z matsunaga $
#
# Copyright (C) 2005-2013 Yusuke Matsunaga
# All rights reserved.


# ===================================================================
# YM_CHECK_QT(action-if-found, action-if-not-found)
#
# Qt を使用するかどうか調べ，使用する場合には実際に
# 使用できるかチェックする．
# ===================================================================
AC_DEFUN([YM_CHECK_QT],[dnl
# autotroll のマクロをそのまま使う．
AT_WITH_QT([], [], [], [ym_tmp_qt=no], [ym_tmp_qt=yes])

# automacro 用のシンボル 'WITH_QT' を定義する．
AM_CONDITIONAL([WITH_QT], [test $ym_tmp_qt = yes])

# マクロ引数を実行する．
AS_IF([test $ym_tmp_qt = yes], [$1], [$2])
])
