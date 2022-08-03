
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( equiv_SOURCES
  EquivMgr.cc
  FraigEnc.cc
  FraigHandle.cc
  FraigMgr.cc
  FraigNode.cc
  FraigSat.cc
  )

set ( luaequiv_SOURCES
  equiv_cmd.cc
  )

# ===================================================================
# ターゲットの設定
# ===================================================================

ym_add_object_library (magus_equiv
  ${equiv_SOURCES}
  )

ym_add_object_library (magus_luaequiv
  ${luaequiv_SOURCES}
  )

target_include_directories ( magus_luaequiv_obj
  PRIVATE ${LUA_INCLUDE_DIR}
  )

target_include_directories ( magus_luaequiv_obj_p
  PRIVATE ${LUA_INCLUDE_DIR}
  )

target_include_directories ( magus_luaequiv_obj_d
  PRIVATE ${LUA_INCLUDE_DIR}
  )
