# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.0)


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
option (OPT_GPERFTOOLS "use google-perftools" OFF)


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)


# ===================================================================
# パッケージの検査
# ===================================================================
list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include (FindDoxygen)

include (FindPythonInterp)
include (FindPythonLibs)

set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_AUTOMOC ON )
find_package(Qt5Widgets)
find_package(OpenGL)

find_package(TCL)

if ( OPT_GPERFTOOLS )
  find_package(Gperftools)
endif (OPT_GPERFTOOLS)

find_package(YmTools REQUIRED)

if (TCL_FOUND)
  find_package(YmTclpp)
endif (TCL_FOUND)


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
  add_subdirectory (src/magus_tclsh)
endif (YmTclpp_FOUND)
