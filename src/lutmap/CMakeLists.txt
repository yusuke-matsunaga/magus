
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
  main/DelayCover.cc
  main/LutMap.cc
  )

set ( enum_cut_SOURCES
  enum_cut/Cut.cc
  enum_cut/CutHolder.cc
  enum_cut/EnumCut.cc
  enum_cut/EnumCutOp.cc
  )

set ( mapgen_SOURCES
  mapgen/MapEst.cc
  mapgen/MapGen.cc
  mapgen/MapRecord.cc
  )

set ( cut_resub_SOURCES
  cut_resub/CrHeap.cc
  cut_resub/CrLevelQ.cc
  cut_resub/CrNode.cc
  cut_resub/CutResubImpl.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library (magus_lutmap
  ${main_SOURCES}
  ${enum_cut_SOURCES}
  ${mapgen_SOURCES}
  ${cut_resub_SOURCES}
  )
