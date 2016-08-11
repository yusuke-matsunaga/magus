
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
  main/LutMap.cc
  main/MapRecord.cc
  )

set ( enum_cut_SOURCES
  enum_cut/Cut.cc
  enum_cut/CutHolder.cc
  enum_cut/EnumCut.cc
  enum_cut/EnumCutOp.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

ym_add_object_library (magus_lutmap3
  ${main_SOURCES}
  ${enum_cut_SOURCES}
  )
