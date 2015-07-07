
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
#  release ターゲットの設定
# ===================================================================

add_library (magus_seal
  STATIC
  ${SOURCES}
  )

target_compile_options (magus_seal
  PRIVATE "-O3"
  )

target_compile_definitions (magus_seal
  PRIVATE "-DNDEBUG"
  )


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================

add_library (magus_seal_p
  STATIC
  ${SOURCES}
  )

target_compile_options (magus_seal_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (magus_seal_p
  PRIVATE "-DNDEBUG"
  )


# ===================================================================
#  debug ターゲットの設定
# ===================================================================

add_library (magus_seal_d
  STATIC
  ${SOURCES}
  )

target_compile_options (magus_seal_d
  PRIVATE "-g"
  )
