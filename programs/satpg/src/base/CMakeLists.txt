
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ../../include
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (SOURCES
  FaultMgr.cc
  TestVector.cc
  TpgFault.cc
  TpgMap.cc
  TpgNetwork.cc
  TpgNode.cc
  TvMgr.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(satpg_base STATIC ${SOURCES})

target_link_libraries(satpg_base
  ym_networks
  )
