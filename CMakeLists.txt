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

# 名前空間
set (SATPG_NAMESPACE "nsMagus")

# ライブラリのインストールディレクトリ
set ( YM_INSTALL_LIBDIR "lib/magus" )


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

# Python のバージョン指定
set( Python_ADDITIONAL_VERSIONS 3.6 3.5 3.4 3.3 3.2 3.1 )
include( FindPythonInterp )
include( FindPythonLibs )

include( UseCython )

find_package(Gperftools)

include (YmUtils)

ym_init ("")


# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
ym_use_gtest ()


# ===================================================================
# CTest モジュールの読み込み
# ===================================================================
include (CTest)

enable_testing()


# ===================================================================
# ヘッダファイルの生成
# ===================================================================

configure_file(
  "${PROJECT_SOURCE_DIR}/etc/version_config.h.in"
  "${PROJECT_BINARY_DIR}/version_config.h"
  )


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-bnet/include
  ${PROJECT_SOURCE_DIR}/ym-cell/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-logic/include
  ${PROJECT_SOURCE_DIR}/ym-mincov/include
  ${PROJECT_SOURCE_DIR}/ym-mvn/include
  ${PROJECT_SOURCE_DIR}/ym-sat/include
  ${PROJECT_SOURCE_DIR}/ym-tclpp/include
  ${PROJECT_SOURCE_DIR}/ym-udgraph/include
  ${PROJECT_SOURCE_DIR}/ym-verilog/include
  ${PROJECT_BINARY_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( ym-common )
add_subdirectory ( ym-logic )
add_subdirectory ( ym-cell )
add_subdirectory ( ym-bnet )
add_subdirectory ( ym-mincov )
add_subdirectory ( ym-mvn )
add_subdirectory ( ym-sat )
add_subdirectory ( ym-tclpp )
add_subdirectory ( ym-udgraph )
add_subdirectory ( ym-verilog )

add_subdirectory (src/cequiv)
add_subdirectory (src/seal)
add_subdirectory (src/lutmap)
add_subdirectory (src/cellmap)

add_subdirectory (magus_tclsh)
