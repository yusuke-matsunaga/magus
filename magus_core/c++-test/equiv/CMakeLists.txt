
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
  $<TARGET_OBJECTS:magus_equiv_ad>
  $<TARGET_OBJECTS:ym_base_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  $<TARGET_OBJECTS:ym_sat_ad>
  $<TARGET_OBJECTS:ym_cell_ad>
  $<TARGET_OBJECTS:ym_bnet_ad>
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
