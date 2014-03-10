# Process this file with autoconf to produce a configure script

# ===================================================================
# Initial block
# ===================================================================
AC_INIT([libym_utils], [2.0],
        [yusuke_matsunaga@ieee.org],
        [libym_utils])dnl

AC_REVISION($Rev: 2507 $)dnl

AC_PREREQ(2.69)dnl

AC_COPYRIGHT([
Copyright (C) 2005-2014 Yusuke Matsunaga
All rights reserved.
])dnl

AC_CONFIG_SRCDIR([include/ymassert.h])dnl

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

if test $enable_shared = no; then
  AC_DEFINE(BUILD_STATIC, 1, [Define for doing static link])
  AM_LDFLAGS="-static"
  AC_SUBST(AM_LDFLAGS)
fi

# GCC のバージョンを $ym_gnuc に入れておく．
AC_COMPUTE_INT(ym_gnuc, [__GNUC__], [], [ym_gnuc=0])
AC_COMPUTE_INT(ym_gnuc_minor, [__GNUC_MINOR__], [], [ym_gnuc_minor=0])


# ===================================================================
# Checks for libraries
# ===================================================================

AC_CHECK_LIB([z],    [compress],          [ym_have_libz=yes])
AC_CHECK_LIB([bz2],  [BZ2_bzCompress],    [ym_have_libbz2=yes])
AC_CHECK_LIB([lzma], [lzma_easy_encoder], [ym_have_liblzma=yes])


# ===================================================================
# Checks for header files
# ===================================================================

AC_HEADER_STDC
AC_HEADER_TIME

AC_CHECK_HEADERS([strings.h],  [ym_have_strings_h=yes])
AC_CHECK_HEADERS([unistd.h],   [ym_have_unistd_h=yes])
AC_CHECK_HEADERS([limits.h],   [ym_have_limits_h=yes])
AC_CHECK_HEADERS([float.h],    [ym_have_float_h=yes])
AC_CHECK_HEADERS([math.h],     [ym_have_math_h=yes])
AC_CHECK_HEADERS([sys/time.h], [ym_have_sys_time_h=yes])
AC_CHECK_HEADERS([sys/stat.h], [ym_have_sys_stat_h=yes])
AC_CHECK_HEADERS([pwd.h])
AC_CHECK_HEADERS([sys/param.h])

AC_CHECK_HEADERS([zlib.h],     [ym_have_zlib_h=yes],  [ym_have_zlib_h=no])
AC_CHECK_HEADERS([bzlib.h],    [ym_have_bzlib_h=yes], [ym_have_bzlib_h=no])
AC_CHECK_HEADERS([lzma.h],     [ym_have_lzma_h=yes],  [ym_have_lzma_h=no])


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
# Checks for library functions
# ===================================================================
#AC_FUNC_ALLOCA
AC_CHECK_FUNCS([random lrand48], break)
AC_CHECK_FUNCS([getrusage times], break)
AC_CHECK_FUNCS([getcwd gettimeofday strtod])


# ===================================================================
# Checks for other packages
# ===================================================================

# Check for zlib
if test "x$ym_have_zlib_h" = xyes -a "x$ym_have_libz" = xyes; then
   ym_have_zlib=yes
else
   ym_have_zlib=no
fi

# Check for bzlib
if test "x$ym_have_bzlib_h" = xyes -a "x$ym_have_libbz2" = xyes; then
   ym_have_bz2=yes
else
   ym_have_bz2=no
fi

# Check for liblzma
if test "x$ym_have_lzma_h" = xyes -a "x$ym_have_liblzma" = xyes; then
   ym_have_lzma=yes
else
   ym_have_lzma=no
fi


# ===================================================================
# Check for Python environment
# ===================================================================
YM_CHECK_PYTHON([ym_have_python=yes], [ym_have_python=no])


# ===================================================================
# Check for C++ language
# ===================================================================
AC_LANG_CPLUSPLUS


# ===================================================================
# Check for CppUnit
# ===================================================================
AM_PATH_CPPUNIT([1.12], [ym_have_cppunit=yes], [ym_have_cppunit=no])


# ===================================================================
# Check for CppUTest
# ===================================================================
YM_CHECK_CPPUTEST([ym_have_cpputest=yes], [ym_have_cpputest=no])


AC_CACHE_SAVE


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
/// Copyright (C) 2005-2014 Yusuke Matsunaga
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

if test $ym_gnuc -ge 4 -a $ym_gnuc_minor -ge 6; then
  cat >> $ymtools << ___EOF___
#include <ext/algorithm>
#include <functional>
#include <unordered_set>
#include <unordered_map>
#define HASH_NAMESPACE std
___EOF___
elif test $ym_gnuc -ge 3; then
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
#include "ymnsdef.h"
#include "ymassert.h"
#include "ymmath.h"
___EOF___

if test $ym_have_zlib=yes; then
  cat >> $ymtools << ___EOF___
/* Define if you have zlib.h */
#define HAVE_ZLIB 1
___EOF___
fi

if test $ym_have_bz2=yes; then
  cat >> $ymtools << ___EOF___
/* Define if you have bzlib.h */
#define HAVE_BZLIB 1
___EOF___
fi

if test $ym_have_lzma=yes; then
  cat >> $ymtools << ___EOF___
/* Define if you have lzma.h */
#define HAVE_LZMA 1
___EOF___
fi

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

if test $ym_gnuc -lt 4 || (test $ym_gnuc -eq 4 && test $ym_gnuc_minor -lt 6); then
  cat >> $ymtools << ___EOF___

#define unordered_set hash_set
#define unordered_map hash_map
#define constexpr const

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
___EOF___
fi

cat >> $ymtools << ___EOF___

#endif // YMTOOLS_H
___EOF___

if test ! -d include; then
   mkdir include
fi
if cmp -s $ymtools include/ymtools.h; then
  AC_MSG_NOTICE([ymtools.h is unchanged])
  rm -f $ymtools
else
  mv $ymtools include/ymtools.h
fi
],[
ac_cv_header_stdc=$ac_cv_header_stdc
ac_cv_header_time=$ac_cv_header_time
ym_have_strings_h=$ym_have_strings_h
ym_have_unistd_h=$ym_have_unistd_h
ym_have_limits_h=$ym_have_limits_h
ym_have_float_h=$ym_have_float_h
ym_have_math_h=$ym_have_math_h
ym_have_sys_time_h=$ym_have_sys_time_h
ym_have_sys_stat_h=$ym_have_sys_stat_h
ym_gnuc=$ym_gnuc
ym_gnuc_minor=$ym_gnuc_minor
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
# Declare files to be generated by configure script
# ===================================================================
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([include/Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/alloc/Makefile])
AC_CONFIG_FILES([src/btg/Makefile])
AC_CONFIG_FILES([src/file/Makefile])
AC_CONFIG_FILES([src/gcsolver/Makefile])
AC_CONFIG_FILES([src/gen/Makefile])
AC_CONFIG_FILES([src/io/Makefile])
AC_CONFIG_FILES([src/itvl/Makefile])
AC_CONFIG_FILES([src/mfset/Makefile])
AC_CONFIG_FILES([src/mincov/Makefile])
AC_CONFIG_FILES([src/misc/Makefile])
AC_CONFIG_FILES([src/msg/Makefile])
AC_CONFIG_FILES([src/str/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([tests/misc/Makefile])


# ===================================================================
# Local definitions
# ===================================================================

YMTOOLS_SRCDIR="\$(top_srcdir)/../.."
YMTOOLS_BUILDDIR="\$(top_builddir)/../.."
AC_SUBST([YMTOOLS_SRCDIR])
AC_SUBST([YMTOOLS_BUILDDIR])

AC_SUBST([YM_CXXOPTFLAGS], ["-O3"])
AC_SUBST([YM_CXXDBGFLAGS], ["-g"])
AC_SUBST([YM_CXXPRFFLAGS], ["-O3 -pg"])

YM_INIT_EXTRA_SUBDIRS
YM_INIT_TESTS_SUBDIRS

AM_CONDITIONAL([YM_HAVE_ZLIB],  [test $ym_have_zlib = yes])
AM_CONDITIONAL([YM_HAVE_BZLIB], [test $ym_have_bz2 = yes])
AM_CONDITIONAL([YM_HAVE_LZMA],  [test $ym_have_lzma = yes])

if test $ym_have_python = yes; then
   YM_ADD_TESTS_SUBDIRS([PyUnit])
   AC_CONFIG_FILES([py-sip/configure.py])
   AC_CONFIG_FILES([tests/PyUnit/Makefile])
fi

if test $ym_have_cppunit = yes; then
   YM_ADD_TESTS_SUBDIRS([cppunit])
   AC_CONFIG_FILES([tests/cppunit/Makefile])
   AC_CONFIG_FILES([tests/cppunit/alloc/Makefile])
   AC_CONFIG_FILES([tests/cppunit/btg/Makefile])
   AC_CONFIG_FILES([tests/cppunit/file/Makefile])
   AC_CONFIG_FILES([tests/cppunit/gcsolver/Makefile])
   AC_CONFIG_FILES([tests/cppunit/gen/Makefile])
   AC_CONFIG_FILES([tests/cppunit/io/Makefile])
   AC_CONFIG_FILES([tests/cppunit/itvl/Makefile])
   AC_CONFIG_FILES([tests/cppunit/mfset/Makefile])
   AC_CONFIG_FILES([tests/cppunit/mincov/Makefile])
   AC_CONFIG_FILES([tests/cppunit/misc/Makefile])
   AC_CONFIG_FILES([tests/cppunit/msg/Makefile])
   AC_CONFIG_FILES([tests/cppunit/str/Makefile])
fi

if test $ym_have_cpputest = yes; then
   YM_ADD_TESTS_SUBDIRS([CppUTest])
   AC_CONFIG_FILES([tests/CppUTest/Makefile])
fi

YM_FINISH_EXTRA_SUBDIRS
YM_FINISH_TESTS_SUBDIRS


# ===================================================================
# Generate configured files
# ===================================================================
AC_OUTPUT

# End-Of-File
