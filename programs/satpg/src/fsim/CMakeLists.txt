
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
  fop/FopKDet.cc
  fsim2/EventQ.cc
  fsim2/Fsim2.cc
  fsim2/SimNode.cc
  fsim2/SnAnd.cc
  fsim2/SnGate.cc
  fsim2/SnInput.cc
  fsim2/SnOr.cc
  fsim2/SnXor.cc
  fsim3/EventQ.cc
  fsim3/Fsim3.cc
  fsim3/SimNode.cc
  fsim3/SnAnd.cc
  fsim3/SnGate.cc
  fsim3/SnInput.cc
  fsim3/SnOr.cc
  fsim3/SnXor.cc
  fsim3/simnode_test.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(satpg_fsim STATIC ${SOURCES})

#target_link_libraries(satpg_fsim
#  ym_networks
#  )
