
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  include
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (cellmap_SOURCES
  cellmap/AreaCover.cc
  cellmap/CellMap.cc
  cellmap/MapRecord.cc
  cellmap/Match.cc
  cellmap/PatMatcher.cc
  )

set (lutmap_SOURCES
  lutmap/AreaCover.cc
  lutmap/CrHeap.cc
  lutmap/CrLevelQ.cc
  lutmap/CrNode.cc
  lutmap/Cut.cc
  lutmap/CutHolder.cc
  lutmap/CutResub.cc
  lutmap/DelayCover.cc
  lutmap/EnumCut.cc
  lutmap/EnumCutOp.cc
  lutmap/LnGraph.cc
  lutmap/LnGraph_dump.cc
  lutmap/LutMap.cc
  lutmap/MapRecord.cc
  )

set (sbjgraph_SOURCES
  sbjgraph/SbjDumper.cc
  sbjgraph/SbjGraph.cc
  sbjgraph/SbjMinDepth.cc
  )


# ===================================================================
#  release ターゲットの設定
# ===================================================================

add_library (magus_techmap
  STATIC
  ${cellmap_SOURCES}
  ${lutmap_SOURCES}
  ${sbjgraph_SOURCES}
  )

target_compile_options (magus_techmap
  PRIVATE "-O3"
  )

target_compile_definitions (magus_techmap
  PRIVATE "-DNDEBUG"
  )


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================

add_library (magus_techmap_p
  STATIC
  ${cellmap_SOURCES}
  ${lutmap_SOURCES}
  ${sbjgraph_SOURCES}
  )

target_compile_options (magus_techmap_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (magus_techmap_p
  PRIVATE "-DNDEBUG"
  )


# ===================================================================
#  debug ターゲットの設定
# ===================================================================

add_library (magus_techmap_d
  STATIC
  ${cellmap_SOURCES}
  ${lutmap_SOURCES}
  ${sbjgraph_SOURCES}
  )

target_compile_options (magus_techmap_d
  PRIVATE "-g"
  )
