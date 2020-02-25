
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${GTEST_INCLUDE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================



# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

add_executable ( equiv_test
  equiv_test.cc
  $<TARGET_OBJECTS:magus_equiv_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

target_compile_options ( equiv_test
  PRIVATE "-g"
  )

target_compile_definitions ( equiv_test
  PRIVATE "-DDATAPATH=\"${PROJECT_SOURCE_DIR}/magus_core/testdata/\""
  )

target_link_libraries ( equiv_test
  ${YM_LIB_DEPENDS}
  pthread
  ${GTEST_BOTH_LIBRARIES}
  )

add_test ( equiv_test
  equiv_test
  )
