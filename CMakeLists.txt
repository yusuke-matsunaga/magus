# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 2.8)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
project (YmTools)
set (YMTOOLS_VERSION_MAJOR 1)
set (YMTOOLS_VERSION_MINOR 0)

# 名前空間
set (YMTOOLS_NAMESPACE "nsYm")


# ===================================================================
# パッケージの検査
# ===================================================================
include (FindDoxygen)
include (FindPythonInterp)
include (FindPythonLibs)
include (FindQt4)
find_package(Qt4 4.8.0 REQUIRED QtCore QtGui QtXml)
include(${QT_USE_FILE})
include (FindTclsh)
include (FindTclStub)
include (FindBZip2)
include (FindLibLZMA)
include (FindZLIB)
include (FindCurses)

#include (FindGMP)
# Copyright (c) 2008-2010 Kent State University
# Copyright (c) 2011-2012 Texas A&M University
#
# This file is distributed under the MIT License. See the accompanying file
# LICENSE.txt or http://www.opensource.org/licenses/mit-license.php for terms
# and conditions.

# FIXME: How do I find the version of GMP that I want to use?
# What versions are available?

# NOTE: GMP prefix is understood to be the path to the root of the GMP
# installation library.
set(GMP_PREFIX "" CACHE PATH "The path to the prefix of a GMP installation")


find_path(GMP_INCLUDE_DIR gmp.h
	PATHS ${GMP_PREFIX}/include /usr/include /usr/local/include)

find_library(GMP_LIBRARY NAMES gmp
	PATHS ${GMP_PREFIX}/lib /usr/lib /usr/local/lib)


if(GMP_INCLUDE_DIR AND GMP_LIBRARY)
	get_filename_component(GMP_LIBRARY_DIR ${GMP_LIBRARY} PATH)
	set(GMP_FOUND TRUE)
endif()

if(GMP_FOUND)
   if(NOT GMP_FIND_QUIETLY)
      MESSAGE(STATUS "Found GMP: ${GMP_LIBRARY}")
   endif()
elseif(GMP_FOUND)
   if(GMP_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find GMP")
   endif()
endif()

# FindPOPT
find_package(PkgConfig QUIET)
pkg_search_module(PC_POPT QUIET popt)

# Find the include directories
FIND_PATH(POPT_INCLUDE_DIR
    NAMES popt.h
    HINTS
          ${PC_POPT_INCLUDEDIR}
          ${PC_POPT_INCLUDE_DIRS}
    DOC "Path containing the popt.h include file"
    )

FIND_LIBRARY(POPT_LIBRARY
    NAMES popt
    HINTS
          ${PC_POPT_LIBRARYDIR}
          ${PC_POPT_LIBRARY_DIRS}
    DOC "popt library path"
    )

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(POPT
  REQUIRED_VARS POPT_INCLUDE_DIR POPT_LIBRARY
  VERSION_VAR PC_POPT_VERSION)

MARK_AS_ADVANCED(POPT_INCLUDE_DIR POPT_LIBRARY)


# ===================================================================
# ヘッダファイルの検査
# ===================================================================
include (CheckIncludeFileCXX)
check_include_file_cxx (stdlib.h    HAVE_STDLIB_H)
check_include_file_cxx (string.h    HAVE_STRING_H)
check_include_file_cxx (strings.h   HAVE_STRINGS_H)
check_include_file_cxx (unistd.h    HAVE_UNISTD_H)
check_include_file_cxx (limits.h    HAVE_LIMITS_H)
check_include_file_cxx (float.h     HAVE_FLOAT_H)
check_include_file_cxx (math.h      HAVE_MATH_H)
check_include_file_cxx (sys/time.h  HAVE_SYS_TIME_H)
check_include_file_cxx (time.h      HAVE_TIME_H)
check_include_file_cxx (sys/stat.h  HAVE_SYS_STAT_H)
check_include_file_cxx (pwd.h       HAVE_PWD_H)
check_include_file_cxx (sys/param.h HAVE_SYS_PARAM_H)


# ===================================================================
# ライブラリ関数の検査
# ===================================================================
include (CheckFunctionExists)
check_function_exists (getrusage    HAVE_GETRUSAGE)
check_function_exists (times        HAVE_TIMES)


# ===================================================================
# 組み込み型のサイズの検査
# ===================================================================
include (CheckTypeSize)
check_type_size (short       SHORT_SIZE)
check_type_size (int         INT_SIZE)
check_type_size (long        LONG_SIZE)
check_type_size ("long long" LLONG_SIZE)
check_type_size ("void *"    VOIDP_SIZE)


# ===================================================================
# サブディレクトリの設定
# ===================================================================
add_subdirectory (libraries/libym_utils)
add_subdirectory (libraries/libym_logic)
add_subdirectory (libraries/libym_cell)

if (TCL_FOUND)
  add_subdirectory (libraries/libym_tclpp)
endif (TCL_FOUND)

#if (Qt4_FOUND)
#  add_subdirectory (libraries/OthQt4)
#  add_subdirectory (plugins/DispMgr/EzDisp)
#  add_subdirectory (plugins/DispMgr/PatDisp)
#endif (Qt4_FOUND)


# ===================================================================
# 設定ファイルの生成
# ===================================================================
configure_file (
  "${PROJECT_SOURCE_DIR}/etc/YmConfig.h.in"
  "${PROJECT_BINARY_DIR}/YmConfig.h"
  )
