
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/magus_core/c++-src/techmap/include
  ${GTEST_INCLUDE_DIR}
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================



# ===================================================================
#  ターゲットの設定
# ===================================================================

add_executable( read_blif
  read_blif.cc
  $<TARGET_OBJECTS:magus_sbjgraph_ad>
  $<TARGET_OBJECTS:ym_bnet_ad>
  $<TARGET_OBJECTS:ym_cell_ad>
  $<TARGET_OBJECTS:ym_sat_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  $<TARGET_OBJECTS:ym_base_ad>
  )

target_compile_options( read_blif
  PRIVATE "-g"
  )

target_link_libraries( read_blif
  ${YM_LIB_DEPENDS}
  )


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

ym_add_gtest( SbjNodeTest
  SbjNodeTest.cc
  $<TARGET_OBJECTS:magus_sbjgraph_ad>
  $<TARGET_OBJECTS:ym_bnet_ad>
  $<TARGET_OBJECTS:ym_cell_ad>
  $<TARGET_OBJECTS:ym_sat_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  $<TARGET_OBJECTS:ym_base_ad>
  )

ym_add_gtest( SbjHandleTest
  SbjHandleTest.cc
  $<TARGET_OBJECTS:magus_sbjgraph_ad>
  $<TARGET_OBJECTS:ym_bnet_ad>
  $<TARGET_OBJECTS:ym_cell_ad>
  $<TARGET_OBJECTS:ym_sat_ad>
  $<TARGET_OBJECTS:ym_logic_ad>
  $<TARGET_OBJECTS:ym_base_ad>
  )
