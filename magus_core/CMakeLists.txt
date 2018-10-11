
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/ym-base/cython-src
  ${PROJECT_SOURCE_DIR}/ym-logic/cython-src
  ${PROJECT_SOURCE_DIR}/ym-sat/cython-src
  ${PROJECT_SOURCE_DIR}/ym-cell/cython-src
  ${PROJECT_SOURCE_DIR}/ym-bnet/cython-src
#  ${PROJECT_SOURCE_DIR}/ym-fraig/cython-src
  ${PROJECT_SOURCE_DIR}/ym-combopt/cython-src
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory (c++-src/equiv)
add_subdirectory (c++-src/techmap)

add_subdirectory ( cython-src )
add_subdirectory ( debug )

add_subdirectory ( c++-test/equiv )
add_subdirectory ( c++-test/techmap/sbjgraph )
add_subdirectory ( py-test )


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
# ターゲットの設定
# ===================================================================


# ===================================================================
#  cython 用のソースファイルの設定
# ===================================================================

set ( CYTHON_SOURCES
  cython-src/magus_core.pyx
  )

set_source_files_properties (
  ${CYTHON_SOURCES}
  PROPERTIES CYTHON_IS_CXX TRUE
  )


# ===================================================================
#  module ターゲットの設定
# ===================================================================

cython_add_module ( magus_core
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:magus_equiv>
  $<TARGET_OBJECTS:magus_cellmap>
  $<TARGET_OBJECTS:magus_lutmap>
  $<TARGET_OBJECTS:magus_sbjgraph>
  $<TARGET_OBJECTS:ym_base>
  $<TARGET_OBJECTS:ym_logic>
  $<TARGET_OBJECTS:ym_cell>
  $<TARGET_OBJECTS:ym_bnet>
  $<TARGET_OBJECTS:ym_fraig>
  $<TARGET_OBJECTS:ym_combopt>
  $<TARGET_OBJECTS:ym_sat>
  )

target_compile_options ( magus_core
  PRIVATE "-O3"
  )

target_compile_definitions ( magus_core
  PRIVATE "-DNDEBUG"
  )

target_link_libraries ( magus_core
  ${YM_LIB_DEPENDS}
  )


cython_add_module ( magus_core_d
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_d>
  $<TARGET_OBJECTS:magus_cellmap_d>
  $<TARGET_OBJECTS:magus_lutmap_d>
  $<TARGET_OBJECTS:magus_sbjgraph_d>
  $<TARGET_OBJECTS:ym_base_d>
  $<TARGET_OBJECTS:ym_logic_d>
  $<TARGET_OBJECTS:ym_cell_d>
  $<TARGET_OBJECTS:ym_bnet_d>
  $<TARGET_OBJECTS:ym_fraig_d>
  $<TARGET_OBJECTS:ym_combopt>
  $<TARGET_OBJECTS:ym_sat_d>
  )

target_compile_options ( magus_core_d
  PRIVATE "-g"
  )

target_link_libraries ( magus_core_d
  ${YM_LIB_DEPENDS}
  )

add_custom_command (
  TARGET magus_core_d
  POST_BUILD
  COMMAND ${CMAKE_COMMAND}
  ARGS -E copy $<TARGET_FILE:magus_core_d> ${CMAKE_CURRENT_BINARY_DIR}/debug/magus_core.so
  )


# ===================================================================
#  テストの設定
# ===================================================================


# ===================================================================
#  インストールターゲットの設定
# ===================================================================
