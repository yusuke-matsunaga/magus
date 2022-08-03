
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  )



# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
# equiv_test
# ===================================================================

set ( equiv_test_SOURCES
  equiv_test.cc
  )

add_executable ( equiv_test
  ${equiv_test_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options ( equiv_test
  PRIVATE "-g"
  )

target_link_libraries ( equiv_test
  ${YM_LIB_DEPENDS}
  )

target_include_directories ( equiv_test
  PRIVATE ${PROJECT_SOURCE_DIR}/c++-srcs/equiv
  )


# ===================================================================
# dg_test
# ===================================================================

set ( dg_test_SOURCES
  dg_test.cc
  )

add_executable ( dg_test
  ${dg_test_SOURCES}
  $<TARGET_OBJECTS:magus_dg_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options ( dg_test
  PRIVATE "-g"
  )

target_include_directories ( dg_test
  PRIVATE ${PROJECT_SOURCE_DIR}/c++-srcs/djdec
  )

target_link_libraries ( dg_test
  ${YM_LIB_DEPENDS}
  )
