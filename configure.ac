# Process this file with autoconf to produce a configure script
#
# $Id: configure.ac 2507 2009-10-17 16:24:02Z matsunaga $

# ===================================================================
# Initial block
# ===================================================================
AC_INIT([ymtools], [1.0], 
        [yusuke_matsunaga@ieee.org],
        [ymtools])dnl

AC_REVISION($Rev: 2507 $)dnl

AC_PREREQ(2.57)dnl

AC_COPYRIGHT([
Copyright (C) 2005-2010 Yusuke Matsunaga
All rights reserved.
])dnl

AC_CONFIG_SRCDIR([include/ymnsdef.h])dnl

AC_CONFIG_AUX_DIR([config])dnl
AC_CONFIG_MACRO_DIR([config])dnl
AC_SUBST(ac_aux_dir)dnl

AM_INIT_AUTOMAKE()dnl


# ===================================================================
# Checks for programs
# ===================================================================
AC_ISC_POSIX
AC_PROG_MAKE_SET
AC_PROG_CC
AC_PROG_CXX
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_YACC
AM_PROG_LEX
AC_PROG_LIBTOOL
AC_CHECK_PROG([DOXYGEN],
	      [doxygen], 
	      [doxygen Doxyfile],
	      [@echo \"You'll need to install 'doxygen'\"])
AC_SUBST(DOXYGEN)

if test $enable_shared = no; then
  AC_DEFINE(BUILD_STATIC, 1, [Define for doing static link])
  AM_LDFLAGS="-static"
  AC_SUBST(AM_LDFLAGS)
fi

# GCC のバージョンを $ym_gnuc に入れておく．
_AC_COMPUTE_INT([__GNUC__], ym_gnuc)


# ===================================================================
# Checks for libraries
# ===================================================================

AC_CHECK_LIB([z], [compress], [ym_have_libz=yes])


# ===================================================================
# Checks for header files
# ===================================================================
AC_HEADER_STDC
AC_HEADER_TIME
AC_CHECK_HEADERS([strings.h], [ym_have_strings_h=yes])
AC_CHECK_HEADERS([unistd.h], [ym_have_unistd_h=yes])
AC_CHECK_HEADERS([limits.h], [ym_have_limits_h=yes])
AC_CHECK_HEADERS([float.h], [ym_have_float_h=yes])
AC_CHECK_HEADERS([math.h], [ym_have_math_h=yes])
AC_CHECK_HEADERS([pwd.h], [ym_have_pwd_h=yes])
AC_CHECK_HEADERS([sys/time.h], [ym_have_sys_time_h=yes])
AC_CHECK_HEADERS([sys/stat.h], [ym_have_sys_stat_h=yes])
AC_CHECK_HEADERS([sys/param.h], [ym_have_sys_param_h=yes])
AC_CHECK_HEADERS([zlib.h], [ym_have_zlib_h=yes])


# ===================================================================
# Checks for typedefs, structures, and compiler characteristics
# ===================================================================
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T


# ===================================================================
# Check for size of built-in types
# ===================================================================
AC_CHECK_SIZEOF(char)
AC_CHECK_SIZEOF(short)
AC_CHECK_SIZEOF(int)
AC_CHECK_SIZEOF(long)
AC_CHECK_SIZEOF(long long)
AC_CHECK_SIZEOF(void*)
AC_CHECK_SIZEOF(size_t)

_AC_COMPUTE_INT([__alignof__ (double)], ym_alignof_double)
AC_DEFINE_UNQUOTED(ALIGNOF_DOUBLE, $ym_alignof_double, [Alignment of double])


# ===================================================================
# Check for return type of signal handler
# ===================================================================
AC_TYPE_SIGNAL


# ===================================================================
# Checks for library functions
# ===================================================================
AC_FUNC_ALLOCA
AC_CHECK_FUNCS([random lrand48], break)
AC_CHECK_FUNCS([getrusage times], break)
AC_CHECK_FUNCS([getcwd gettimeofday strtod])


# ===================================================================
# Checks for system services
# ===================================================================
AC_PATH_XTRA

AC_CACHE_SAVE


# ===================================================================
# Checks for other packages
# ===================================================================

# Check for zlib
if test "x$ym_have_zlib_h" = xyes -a "x$ym_have_libz" = xyes; then
   ym_have_zlib=1
else
   ym_have_zlib=0
fi
AC_DEFINE_UNQUOTED([HAVE_ZLIB], $ym_have_zlib, [Define if you have zlib.h])

# Check for gmp
YM_CHECK_GMP

# Check for popt
YM_CHECK_POPT

# Check for TCL environment
YM_CHECK_TCL

# Check for tclreadline
YM_CHECK_TCLREADLINE

AC_CACHE_SAVE


# ===================================================================
# Check for QT
# It's a kind of magic!
# ===================================================================
AC_ARG_ENABLE([qt],
[  --enable-qt    Turn on QT],
[case "${enableval}" in
 yes) qt=true ;;
 no)  qt=false ;;
 *) AC_MSG_ERROR([bad value ${enableval} for --enable-qt]) ;;
 esac],[qt=false])
AM_CONDITIONAL([WITH_QT], [test x$qt = xtrue])
if test x$qt = xtrue; then
  AT_WITH_QT
fi


# ===================================================================
# Generating ymtools.h
# ===================================================================
AC_CONFIG_COMMANDS([ymtools.h],[
ymtools="ymtools.h-tmp"
cat > $ymtools <<___EOF___
#ifndef YMTOOLS_H
#define YMTOOLS_H

/// @file ymtools.h
/// @brief ymtools に共通な型定義ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// Do not edit! This file is generated from configure.in

//////////////////////////////////////////////////////////////////////
// 基本的なヘッダフィルのインクルード
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <sys/types.h>
___EOF___

if test "x$ac_cv_header_stdc" = xyes; then
  cat >> $ymtools << ___EOF___
#include <stdlib.h>
#include <string.h>
___EOF___
elif test "x$ym_have_strings_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <strings.h>
___EOF___
fi

if test "x$ym_have_unistd_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <unistd.h>
___EOF___
fi

if test "x$ym_have_limits_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <limits.h>
___EOF___
fi

if test "x$ym_have_float_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <float.h>
___EOF___
fi

if test "x$ym_have_math_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <math.h>
___EOF___
fi

if test "x$ac_cv_header_time" = xyes; then
  cat >> $ymtools << ___EOF___
#include <sys/time.h>
#include <time.h>
___EOF___
elif test "x$ym_have_sys_time_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <sys/time.h>
___EOF___
else
  cat >> $ymtools << ___EOF___
#include <time.h>
___EOF___
fi

if test "x$ym_have_sys_stat_h" = xyes; then
  cat >> $ymtools << ___EOF___
#include <sys/stat.h>
___EOF___
fi

cat >> $ymtools << ___EOF___
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <exception>
#include <new>
#include <utility>
#include <string>
#include <vector>
#include <list>
___EOF___

if test $ym_gnuc -ge 3; then
  cat >> $ymtools << ___EOF___
#include <ext/algorithm>
#include <ext/functional>
#include <ext/hash_set>
#include <ext/hash_map>
#define HASH_NAMESPACE __gnu_cxx
___EOF___
else
  cat >> $ymtools << ___EOF___
#include <algorithm>
#include <functional>
#include <hash_set>
#include <hash_map>
#define HASH_NAMESPACE std
___EOF___
fi

cat >> $ymtools << ___EOF___
#include <ymnsdef.h>
#include <ymassert.h>
#include <ymmath.h>
___EOF___

cat >> $ymtools << ___EOF___

//////////////////////////////////////////////////////////////////////
// 整数型の定義
//////////////////////////////////////////////////////////////////////

/// @brief 符号付き整数型
typedef int ymint;

/// @brief 符号付きlong型
typedef long ymlong;

/// @brief 符号無し整数型
typedef unsigned int ymuint;

/// @brief 符号無しlong型
typedef unsigned long ymulong;

/// @brief 8ビット符号付き整数型
typedef $ymint8 ymint8;

/// @brief 16ビット符号付き整数型
typedef $ymint16 ymint16;

/// @brief 32ビット符号付き整数型
typedef $ymint32 ymint32;

/// @brief 64ビット符号付き整数型
typedef $ymint64 ymint64;

/// @brief 8ビット符号なし整数型
typedef unsigned $ymint8 ymuint8;

/// @brief 16ビット符号なし整数型
typedef unsigned $ymint16 ymuint16;

/// @brief 32ビット符号なし整数型
typedef unsigned $ymint32 ymuint32;

/// @brief 64ビット符号なし整数型
typedef unsigned $ymint64 ymuint64;

/// @brief ポインタと同じサイズの符号なし整数型
typedef $ymptrint ympuint;

___EOF___

cat >> $ymtools << ___EOF___

BEGIN_NAMESPACE_HASH

// string/const string の hash_set/map 用のハッシュ関数クラス 
// hash<string>, hash<const string>定義．
// SGI STL の string クラス (v3.1 以降)にはこの定義は不要．
#if !defined(__SGI_STL_STRING) && !defined(DEFINE_HASH_STRING)
#define DEFINE_HASH_STRING

template<typename __Key> struct hash;

template <>
struct
hash<std::string>
{
  size_t
  operator()(const std::string& __x) const
  {
    unsigned long __h = 0; 
    for (std::string::const_iterator __s = __x.begin(); __s != __x.end();
	 ++__s)
      __h = 5*__h + *__s;
    
    return size_t(__h);
  }
};

template <>
struct
hash<const std::string>
{
  size_t
  operator()(const std::string& __x) const {
    unsigned long __h = 0; 
    for (std::string::const_iterator __s = __x.begin(); __s != __x.end();
	 ++__s)
      __h = 5*__h + *__s;
    
    return size_t(__h);
  }
};
 
#endif // __SGI_STL_STRING

END_NAMESPACE_HASH


//#include "ymconfig.h"

#endif // YMTOOLS_H
___EOF___

if cmp -s $ymtools ymtools.h; then
  AC_MSG_NOTICE([ymtools.h is unchanged])
  rm -f $ymtools
else
  mv $ymtools ymtools.h
fi
],[
ac_cv_header_stdc=$ac_cv_header_stdc
ac_cv_header_time=$ac_cv_header_time
ym_have_strings_h=$ym_have_strings_h
ym_have_unistd_h=$ym_have_unistd_h
ym_have_limits_h=$ym_have_limits_h
ym_have_float_h=$ym_have_float_h
ym_have_math_h=$ym_have_math_h
ym_have_pwd_h=$ym_have_pwd_h
ym_have_sys_time_h=$ym_have_sys_time_h
ym_have_sys_stat_h=$ym_have_sys_stat_h
ym_have_sys_param_h=$ym_have_sys_param_h
ym_gnuc=$ym_gnuc
ymint8=char
case 2 in
$ac_cv_sizeof_short)
  ymint16=short
  ;;
$ac_cv_sizeof_int)
  ymint16=int
  ;;
esac
case 4 in
$ac_cv_sizeof_short)
  ymint32=short
  ;;
$ac_cv_sizeof_int)
  ymint32=int
  ;;
$ac_cv_sizeof_long)
  ymint32=long
  ;;
esac
case 8 in
$ac_cv_sizeof_int)
  ymint64=int
  ;;
$ac_cv_sizeof_long)
  ymint64=long
  ;;
$ac_cv_sizeof_long_long)
  ymint64='long long'
  ;;
esac
case $ac_cv_sizeof_voidp in
$ac_cv_sizeof_int)
  ymptrint='unsigned int'
  ;;
$ac_cv_sizeof_long)
  ymptrint='unsigned long'
  ;;
$ac_cv_sizeof_long_long)
  ymptrint='unsigned long long'
  ;;
esac
])


# ===================================================================
# Check for C++ language
# ===================================================================
AC_LANG_CPLUSPLUS


# ===================================================================
# Declare files to be generated by configure script
# ===================================================================
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([libraries/Makefile])
AC_CONFIG_FILES([programs/Makefile])
AC_CONFIG_FILES([Doxyfile])


# ===================================================================
# Declare files in 'libraries' subdirectories
# ===================================================================

YM_INIT_LIBRARIES_SUBDIRS

# libym_utils
YM_ADD_LIBRARIES_SUBDIRS([libym_utils])
AC_CONFIG_FILES([libraries/libym_utils/Makefile])
AC_CONFIG_FILES([libraries/libym_utils/ym_utils/Makefile])
AC_CONFIG_FILES([libraries/libym_utils/tests/Makefile])

# libym_zstream
if test "$ym_have_zlib" = 1; then
YM_ADD_LIBRARIES_SUBDIRS([libym_zstream])
AC_CONFIG_FILES([libraries/libym_zstream/Makefile])
AC_CONFIG_FILES([libraries/libym_zstream/ym_zstream/Makefile])
AC_CONFIG_FILES([libraries/libym_zstream/tests/Makefile])
fi

# libym_lexp
YM_ADD_LIBRARIES_SUBDIRS([libym_lexp])
AC_CONFIG_FILES([libraries/libym_lexp/Makefile])
AC_CONFIG_FILES([libraries/libym_lexp/ym_lexp/Makefile])
AC_CONFIG_FILES([libraries/libym_lexp/tests/Makefile])

# libym_bdd
YM_ADD_LIBRARIES_SUBDIRS([libym_bdd])
AC_CONFIG_FILES([libraries/libym_bdd/Makefile])
AC_CONFIG_FILES([libraries/libym_bdd/ym_bdd/Makefile])
AC_CONFIG_FILES([libraries/libym_bdd/base/Makefile])
AC_CONFIG_FILES([libraries/libym_bdd/bmc/Makefile])
AC_CONFIG_FILES([libraries/libym_bdd/bmm/Makefile])
AC_CONFIG_FILES([libraries/libym_bdd/dec/Makefile])
AC_CONFIG_FILES([libraries/libym_bdd/tests/Makefile])

# libym_npn
YM_ADD_LIBRARIES_SUBDIRS([libym_npn])
AC_CONFIG_FILES([libraries/libym_npn/Makefile])
AC_CONFIG_FILES([libraries/libym_npn/ym_npn/Makefile])
AC_CONFIG_FILES([libraries/libym_npn/utils/Makefile])
AC_CONFIG_FILES([libraries/libym_npn/tests/Makefile])

# libym_sat
YM_ADD_LIBRARIES_SUBDIRS([libym_sat])
AC_CONFIG_FILES([libraries/libym_sat/Makefile])
AC_CONFIG_FILES([libraries/libym_sat/ym_sat/Makefile])
AC_CONFIG_FILES([libraries/libym_sat/analyzer/Makefile])
AC_CONFIG_FILES([libraries/libym_sat/MiniSat/Makefile])
AC_CONFIG_FILES([libraries/libym_sat/tests/Makefile])

# libym_mincov
YM_ADD_LIBRARIES_SUBDIRS([libym_mincov])
AC_CONFIG_FILES([libraries/libym_mincov/Makefile])
AC_CONFIG_FILES([libraries/libym_mincov/ym_mincov/Makefile])
AC_CONFIG_FILES([libraries/libym_mincov/tests/Makefile])

# libym_blif
YM_ADD_LIBRARIES_SUBDIRS([libym_blif])
AC_CONFIG_FILES([libraries/libym_blif/Makefile])
AC_CONFIG_FILES([libraries/libym_blif/ym_blif/Makefile])
AC_CONFIG_FILES([libraries/libym_blif/tests/Makefile])

# libym_iscas89
YM_ADD_LIBRARIES_SUBDIRS([libym_iscas89])
AC_CONFIG_FILES([libraries/libym_iscas89/Makefile])
AC_CONFIG_FILES([libraries/libym_iscas89/ym_iscas89/Makefile])
AC_CONFIG_FILES([libraries/libym_iscas89/tests/Makefile])

# libym_cell
YM_ADD_LIBRARIES_SUBDIRS([libym_cell])
AC_CONFIG_FILES([libraries/libym_cell/Makefile])
AC_CONFIG_FILES([libraries/libym_cell/ym_cell/Makefile])
AC_CONFIG_FILES([libraries/libym_cell/mislib/Makefile])
AC_CONFIG_FILES([libraries/libym_cell/dotlib/Makefile])
AC_CONFIG_FILES([libraries/libym_cell/tests/Makefile])

# libym_verilog
YM_ADD_LIBRARIES_SUBDIRS([libym_verilog])
AC_CONFIG_FILES([libraries/libym_verilog/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/ym_verilog/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/ym_verilog/pt/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/ym_verilog/vl/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/common/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/parser/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/parser/include/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/parser/main/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/parser/scanner/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/parser/pt_mgr/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/parser/spt/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/parser/cpt/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/elaborator/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/elaborator/include/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/elaborator/main/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/elaborator/elb_mgr/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/elaborator/ei/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/tests/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/tests/vltest/Makefile])
AC_CONFIG_FILES([libraries/libym_verilog/tests/vlview/Makefile])

# libym_bnet
YM_ADD_LIBRARIES_SUBDIRS([libym_bnet])
AC_CONFIG_FILES([libraries/libym_bnet/Makefile])
AC_CONFIG_FILES([libraries/libym_bnet/ym_bnet/Makefile])
AC_CONFIG_FILES([libraries/libym_bnet/tests/Makefile])

# libym_bdn
YM_ADD_LIBRARIES_SUBDIRS([libym_bdn])
AC_CONFIG_FILES([libraries/libym_bdn/Makefile])
AC_CONFIG_FILES([libraries/libym_bdn/ym_bdn/Makefile])
AC_CONFIG_FILES([libraries/libym_bdn/tests/Makefile])

# libym_aig
YM_ADD_LIBRARIES_SUBDIRS([libym_aig])
AC_CONFIG_FILES([libraries/libym_aig/Makefile])
AC_CONFIG_FILES([libraries/libym_aig/ym_aig/Makefile])
AC_CONFIG_FILES([libraries/libym_aig/tests/Makefile])

# libym_tgnet
YM_ADD_LIBRARIES_SUBDIRS([libym_tgnet])
AC_CONFIG_FILES([libraries/libym_tgnet/Makefile])
AC_CONFIG_FILES([libraries/libym_tgnet/ym_tgnet/Makefile])
AC_CONFIG_FILES([libraries/libym_tgnet/tests/Makefile])

# libym_gds
YM_ADD_LIBRARIES_SUBDIRS([libym_gds])
AC_CONFIG_FILES([libraries/libym_gds/Makefile])
AC_CONFIG_FILES([libraries/libym_gds/ym_gds/Makefile])
AC_CONFIG_FILES([libraries/libym_gds/tests/Makefile])

# libym_ceq
YM_ADD_LIBRARIES_SUBDIRS([libym_ceq])
AC_CONFIG_FILES([libraries/libym_ceq/Makefile])
AC_CONFIG_FILES([libraries/libym_ceq/ym_ceq/Makefile])
AC_CONFIG_FILES([libraries/libym_ceq/tests/Makefile])

# libym_lutmap
YM_ADD_LIBRARIES_SUBDIRS([libym_lutmap])
AC_CONFIG_FILES([libraries/libym_lutmap/Makefile])
AC_CONFIG_FILES([libraries/libym_lutmap/ym_lutmap/Makefile])
AC_CONFIG_FILES([libraries/libym_lutmap/tests/Makefile])

# libym_tclpp
YM_ADD_LIBRARIES_SUBDIRS([libym_tclpp])
AC_CONFIG_FILES([libraries/libym_tclpp/Makefile])
AC_CONFIG_FILES([libraries/libym_tclpp/ym_tclpp/Makefile])
AC_CONFIG_FILES([libraries/libym_tclpp/examples/Makefile])

YM_FINISH_LIBRARIES_SUBDIRS


# ===================================================================
# Declare files in 'programs' subdirectories
# ===================================================================

YM_INIT_PROGRAMS_SUBDIRS

# magus
YM_ADD_PROGRAMS_SUBDIRS([magus])
AC_CONFIG_SUBDIRS([programs/magus])

# atpg
YM_ADD_PROGRAMS_SUBDIRS([atpg])
AC_CONFIG_SUBDIRS([programs/atpg])

# matpg
YM_ADD_PROGRAMS_SUBDIRS([matpg])
AC_CONFIG_SUBDIRS([programs/matpg])

YM_FINISH_PROGRAMS_SUBDIRS


# ===================================================================
# Generate configured files
# ===================================================================
AC_OUTPUT

# End-Of-File
