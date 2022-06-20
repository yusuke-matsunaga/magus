
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/c++-srcs/techmap/include
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
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
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
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )

ym_add_gtest( SbjHandleTest
  SbjHandleTest.cc
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
  )
