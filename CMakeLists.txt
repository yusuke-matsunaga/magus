# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.0)


# C++11 の指定
set (CMAKE_CXX_STANDARD 11)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
project (ym-magus)
set (YM_MAGUS_VERSION_MAJOR 1)
set (YM_MAGUS_VERSION_MINOR 0)
set (YM_MAGUS_VERSION ${YM_MAGUS_VERSION_MAJOR}.${YM_MAGUS_VERSION_MINOR})


# ===================================================================
# オプション
# ===================================================================


# ===================================================================
# パッケージの検査
# ===================================================================
list ( APPEND CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/ym-common/cmake"
  )

include (FindDoxygen)

#include (FindPythonInterp)
#include (FindPythonLibs)

include (ym_add_library)

#set( CMAKE_INCLUDE_CURRENT_DIR ON )
#find_package(Qt5Widgets)
#find_package(OpenGL)

find_package(TCL)

find_package(Gperftools)

find_package(YmTools REQUIRED)

if (TCL_FOUND)
  find_package(YmTclpp)
endif (TCL_FOUND)

include (ym_add_library)


# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
include ( ym_use_gtest )
ym_use_gtest ()


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)

enable_testing()


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ${YmTools_INCLUDE_DIRS}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================
add_subdirectory (src/cequiv)
add_subdirectory (src/seal)
add_subdirectory (src/techmap)

if (YmTclpp_FOUND)
  add_subdirectory (magus_tclsh)
endif (YmTclpp_FOUND)
