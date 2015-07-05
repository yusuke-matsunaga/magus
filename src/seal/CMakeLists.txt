
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ../../include
#  ${GMP_INCLUDE_DIR}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (SOURCES
  BddFsm.cc
  Dfs.cc
  LogicSim.cc
  MCAnalysis.cc
  Matrix.cc
  SMatrix.cc
  fsm_analysis.cc
  fsm_analysis2.cc
  gauss_elimination.cc
  lu_decomp.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(magus_seal STATIC ${SOURCES})
