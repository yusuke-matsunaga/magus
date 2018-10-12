# ===================================================================
# CMAKE のおまじない
# ===================================================================
cmake_minimum_required (VERSION 3.2)

# C++14 の指定
set (CMAKE_CXX_STANDARD 14)


# ===================================================================
# プロジェクト名，バージョンの設定
# ===================================================================
set ( YM_VERSION_MAJOR 1 )
set ( YM_VERSION_MINOR 0 )
set ( YM_VERSION ${YM_VERSION_MAJOR}.${YM_VERSION_MINOR})
project ( magus VERSION "${YM_VERSION}")

set ( YM_PROJECT_STRING "${PROJECT_NAME}-${PROJECT_VERSION}" )

# 名前空間
set (MAGUS_NSNAME "nsMagus")

# インストールディレクトリ
set ( CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/stage" )


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
include( UseCython )
find_package(Gperftools)

include (YmUtils)

ym_init ("")

ym_init_python ()

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
# スクリプトファイルの生成
# ===================================================================

if ( PYTHONINTERP_FOUND )
  configure_file(
    "${PROJECT_SOURCE_DIR}/etc/ym_python.in"
    "${PROJECT_BINARY_DIR}/ym_python"
    @ONLY
    )

  install ( PROGRAMS ${PROJECT_BINARY_DIR}/ym_python
    DESTINATION bin
    )
endif ()


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/ym-base/include
  ${PROJECT_SOURCE_DIR}/ym-bnet/include
  ${PROJECT_SOURCE_DIR}/ym-cell/include
  ${PROJECT_SOURCE_DIR}/ym-combopt/include
  ${PROJECT_SOURCE_DIR}/ym-common/include
  ${PROJECT_SOURCE_DIR}/ym-fraig/include
  ${PROJECT_SOURCE_DIR}/ym-logic/include
  ${PROJECT_SOURCE_DIR}/ym-mvn/include
  ${PROJECT_SOURCE_DIR}/ym-sat/include
  ${PROJECT_SOURCE_DIR}/ym-tclpp/include
  ${PROJECT_SOURCE_DIR}/ym-verilog/include
  ${PROJECT_BINARY_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory ( ym-common )
add_subdirectory ( ym-base )
add_subdirectory ( ym-combopt )
add_subdirectory ( ym-logic )
add_subdirectory ( ym-cell )
add_subdirectory ( ym-bnet )
add_subdirectory ( ym-fraig )
add_subdirectory ( ym-mvn )
add_subdirectory ( ym-sat )
add_subdirectory ( ym-tclpp )
add_subdirectory ( ym-verilog )

add_subdirectory ( magus_core )
add_subdirectory ( magus_tclsh )

if (DOXYGEN_FOUND)

  # doxygen の対象となるヘッダファイル(のパタン)を YM_DOXY_SRCS に入れる．
  set ( YM_DOXY_SRCS )
  foreach ( dir
      "${PROJECT_SOURCE_DIR}/doxygen-src"
      "${PROJECT_BINARY_DIR}"
      )
    list ( APPEND YM_DOXY_SRCS ${dir} )
  endforeach ()
  foreach ( dir
      "magus_core"
      #"magus_tclsh"
      "ym-bnet"
      "ym-cell"
      "ym-combopt"
      "ym-common"
      "ym-fraig"
      "ym-logic"
      "ym-mvn"
      "ym-sat"
      "ym-tclpp"
      "ym-verilog"
      )
    list ( APPEND YM_DOXY_SRCS "${PROJECT_SOURCE_DIR}/${dir}/include" )
  endforeach ()

  ym_add_doxygen ()

endif (DOXYGEN_FOUND)


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
