
# ===================================================================
# インクルードパスの設定
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================


# ===================================================================
#  ターゲットの設定
# ===================================================================


# ===================================================================
#  テスト用のターゲットの設定
# ===================================================================

add_test ( NAME lua_magus_bnet_test
  COMMAND lua_magus ${CMAKE_CURRENT_SOURCE_DIR}/bnet-test.lua
  )

add_test ( NAME lua_magus_equiv_test
  COMMAND lua_magus ${CMAKE_CURRENT_SOURCE_DIR}/equiv-test.lua
  )

add_test ( NAME lua_magus_equiv_test2
  COMMAND lua_magus ${CMAKE_CURRENT_SOURCE_DIR}/equiv-test2.lua
  )
