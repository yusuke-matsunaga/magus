
# ===================================================================
# インクルードパスの設定
# ===================================================================

include_directories (
  ${YM_SUBMODULE_INCLUDE_LIST}
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


if ( 0 )
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

list ( REMOVE_ITEM YM_SUBMODULE_OBJ_LIST "$<TARGET_OBJECTS:ym_tclpp>" )
cython_add_module ( magus_core
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj>
  $<TARGET_OBJECTS:magus_cellmap_obj>
  $<TARGET_OBJECTS:magus_lutmap_obj>
  $<TARGET_OBJECTS:magus_sbjgraph_obj>
  ${YM_SUBMODULE_OBJ_LIST}
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


list ( REMOVE_ITEM YM_SUBMODULE_OBJ_D_LIST "$<TARGET_OBJECTS:ym_tclpp_d>" )
cython_add_module ( magus_core_d
  ${CYTHON_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_d>
  $<TARGET_OBJECTS:magus_cellmap_obj_d>
  $<TARGET_OBJECTS:magus_lutmap_obj_d>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
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

install ( TARGETS magus_core
  DESTINATION "${YM_MAGUS_PKGDIR}"
  )
endif ()
