
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

set (sbjgraph_SOURCES
  sbjgraph/SbjDumper.cc
  sbjgraph/SbjGraph.cc
  sbjgraph/SbjMinDepth.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library (magus_techmap
  ${cellmap_SOURCES}
  )
