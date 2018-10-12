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

include (YmUtils)

ym_init ("")
ym_init_python ()
ym_init_cython ()
ym_init_doxygen ()
ym_init_gperftools ()
ym_init_ctest ()

# ===================================================================
# google-test は内蔵のものを使う．
# ===================================================================
ym_use_gtest ()


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
# サブモジュールのリスト
# ===================================================================

ym_init_submodules (
  ym-base
  #  "ym-bfo
  ym-bnet
  ym-cell
  ym-combopt
  ym-common
  ym-fraig
  ym-logic
  ym-mvn
  ym-sat
  ym-tclpp
  ym-verilog
  )


# ===================================================================
# インクルードパスの設定
# ===================================================================

include_directories( "${PROJECT_BINARY_DIR}" ${YM_SUBMODULE_INCLUDE_LIST} )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

foreach ( dir ${YM_SUBMODULE_LIST} )
  add_subdirectory ( ${dir} )
endforeach ()

add_subdirectory ( magus_core )
add_subdirectory ( magus_tclsh )


# ===================================================================
# Doxygen の設定
# ===================================================================

if (DOXYGEN_FOUND)

  # doxygen の対象となるヘッダファイル(のパタン)を YM_DOXY_SRCS に入れる．
  set ( YM_DOXY_SRCS )
  foreach ( dir
      "${PROJECT_SOURCE_DIR}/doxygen-src"
      "${PROJECT_BINARY_DIR}"
      )
    list ( APPEND YM_DOXY_SRCS ${dir} )
  endforeach ()
  foreach ( dir ${YM_SUBMODULE_LIST} )
    list ( APPEND YM_DOXY_SRCS "${PROJECT_SOURCE_DIR}/${dir}/include" )
  endforeach ()
  list ( APPEND YM_DOXY_SRCS "${PROJECT_SOURCE_DIR}/magus_core/include" )
  #list ( APPEND YM_DOXY_SRCS "${PROJECT_SOURCE_DIR}/magus_tclsh/include" )

  ym_add_doxygen ()

endif (DOXYGEN_FOUND)


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
