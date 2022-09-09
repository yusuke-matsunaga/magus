
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  include
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set ( enum_cut_SOURCES
  enum_cut/Cut.cc
  enum_cut/CutHolder.cc
  enum_cut/EnumCut.cc
  enum_cut/EnumCutOp.cc
  )

set ( main_SOURCES
  main/AreaCover.cc
  main/DelayCover.cc
  main/LbCalc.cc
  main/DgGraph.cc
  main/MapGen.cc
  main/lutmap_cmd.cc
  )

set ( mct1_SOURCES
  mct1/MctNode.cc
  mct1/MctSearch.cc
  mct1/MctState.cc
  )

set ( mct2_SOURCES
  mct2/MctNode.cc
  mct2/MctSearch.cc
  mct2/MctState.cc
  )

set ( sa_SOURCES
  sa/SaSearch.cc
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

ym_add_object_library (magus_lualutmap
  ${enum_cut_SOURCES}
  ${main_SOURCES}
  ${mct1_SOURCES}
  ${mct2_SOURCES}
  ${sa_SOURCES}
  ${cut_resub_SOURCES}
  )

target_include_directories ( magus_lualutmap_obj
  PRIVATE ${LUA_INCLUDE_DIR}
  )

target_include_directories ( magus_lualutmap_obj_p
  PRIVATE ${LUA_INCLUDE_DIR}
  )

target_include_directories ( magus_lualutmap_obj_d
  PRIVATE ${LUA_INCLUDE_DIR}
  )
