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

# デバッグモード
set ( CMAKE_BUILD_TYPE DEBUG )


# ===================================================================
# パッケージの検査
# ===================================================================
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (FindDoxygen)

find_package(BISON REQUIRED)
include (FindPythonInterp)
include (FindPythonLibs)

include (FindQt4)
find_package(Qt4 4.8.0 REQUIRED QtCore QtGui QtXml)
include(${QT_USE_FILE})
find_package(TCL)
#include (FindTclsh)
#include (FindTclStub)
include (FindBZip2)
include (FindLibLZMA)
include (FindZLIB)
include (FindCurses)

include (FindGMP)

include (FindPOPT)

include (FindReadline)


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
add_subdirectory (libraries/libym_verilog)
add_subdirectory (libraries/libym_networks)
add_subdirectory (libraries/libym_ymsh)

if (TCL_FOUND)
  add_subdirectory (libraries/libym_tclpp)
endif (TCL_FOUND)

add_subdirectory (programs/bnet2aig)
add_subdirectory (programs/makebdd)
add_subdirectory (programs/satpg)
add_subdirectory (programs/magus)


# ===================================================================
# 設定ファイルの生成
# ===================================================================
configure_file (
  "${PROJECT_SOURCE_DIR}/etc/YmConfig.h.in"
  "${PROJECT_BINARY_DIR}/YmConfig.h"
  )
