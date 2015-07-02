# ===================================================================
# libym_algo のための CMakeLists.txt
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================

if (GTEST_FOUND)
  add_subdirectory(gtest)
endif (GTEST_FOUND)


# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
set (btg_SOURCES
  src/btg/BtgHeapTree.cc
  src/btg/BtgMatch.cc
  src/btg/BtgMatchImpl.cc
  )

set (graph_SOURCES
  src/graph/GcSolver.cc
  src/graph/Graph.cc
  src/graph/independent_set.cc
  src/graph/max_clique.cc
  src/graph/max_clique_exact.cc
  )

set (mincov_SOURCES
  src/mincov/LbCS.cc
  src/mincov/LbMAX.cc
  src/mincov/LbMIS1.cc
  src/mincov/LbMIS2.cc
  src/mincov/LbMIS3.cc
  src/mincov/MaxClique.cc
  src/mincov/McMatrix.cc
  src/mincov/McSolver.cc
  src/mincov/McSolverImpl.cc
  src/mincov/MinCov.cc
  src/mincov/MisNodeHeap.cc
  src/mincov/SelCS.cc
  src/mincov/SelNaive.cc
  src/mincov/SelSimple.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_library(ym_algo
  ${btg_SOURCES}
  ${graph_SOURCES}
  ${mincov_SOURCES}
  )
