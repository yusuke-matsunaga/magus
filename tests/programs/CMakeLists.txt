
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
