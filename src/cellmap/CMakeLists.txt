
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  include
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( main_SOURCES
  main/AreaCover.cc
  main/CellMap.cc
  )

set ( match_SOURCES
  match/Cut.cc
  match/PatMatcher.cc
  )

set ( mapgen_SOURCES
  mapgen/MapGen.cc
  mapgen/MapRecord.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library (magus_cellmap
  ${main_SOURCES}
  ${match_SOURCES}
  ${mapgen_SOURCES}
  )
