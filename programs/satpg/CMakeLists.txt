
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  include
  )

if (TCL_FOUND)
  include_directories ( ${TCL_INCLUDE_PATH} )
endif (TCL_FOUND)

# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (base_SOURCES
  src/base/FaultMgr.cc
  src/base/NodeSet.cc
  src/base/NodeValList.cc
  src/base/TestVector.cc
  src/base/TpgFault.cc
  src/base/TpgMap.cc
  src/base/TpgNetwork.cc
  src/base/TpgNode.cc
  src/base/TvMgr.cc
  )

set (dtpg_SOURCES
  src/dtpg/bt/BtBase.cc
  src/dtpg/bt/BtJust1.cc
  src/dtpg/bt/BtJust2.cc
  src/dtpg/bt/BtJustBase.cc
  src/dtpg/bt/BtSimple.cc

  src/dtpg/dop/DopBase.cc
  src/dtpg/dop/DopDrop.cc
  src/dtpg/dop/DopDummy.cc
  src/dtpg/dop/DopList.cc
  src/dtpg/dop/DopTvList.cc
  src/dtpg/dop/DopVerify.cc
  src/dtpg/dop/FopDrop.cc

  src/dtpg/engine/SatEngine.cc
  src/dtpg/engine/VidLitMap.cc

  src/dtpg/dtpg_engine/Extractor.cc
  src/dtpg/dtpg_engine/LitMap.cc
  src/dtpg/dtpg_engine/ModelValMap.cc
  src/dtpg/dtpg_engine/GenVidMap.cc
  src/dtpg/dtpg_engine/DtpgSat.cc
  src/dtpg/dtpg_engine/DtpgSatBaseS.cc
  src/dtpg/dtpg_engine/DtpgSatBaseM.cc
  src/dtpg/dtpg_engine/DtpgSatS.cc
  src/dtpg/dtpg_engine/DtpgSatS2.cc
  src/dtpg/dtpg_engine/DtpgSatM.cc
  src/dtpg/dtpg_engine/DtpgSatM2.cc
  src/dtpg/dtpg_engine/DtpgSatKDet.cc
  src/dtpg/dtpg_engine/DtpgSatC.cc

  src/dtpg/uop/UopBase.cc
  src/dtpg/uop/UopDummy.cc
  src/dtpg/uop/UopList.cc
  src/dtpg/uop/UopSkip.cc
  )

set (fsim_SOURCES
  src/fsim/fop/FopKDet.cc

  src/fsim/fsim2/EventQ.cc
  src/fsim/fsim2/Fsim2.cc
  src/fsim/fsim2/SimNode.cc
  src/fsim/fsim2/SnAnd.cc
  src/fsim/fsim2/SnGate.cc
  src/fsim/fsim2/SnInput.cc
  src/fsim/fsim2/SnOr.cc
  src/fsim/fsim2/SnXor.cc

  src/fsim/fsim3/EventQ.cc
  src/fsim/fsim3/Fsim3.cc
  src/fsim/fsim3/SimNode.cc
  src/fsim/fsim3/SnAnd.cc
  src/fsim/fsim3/SnGate.cc
  src/fsim/fsim3/SnInput.cc
  src/fsim/fsim3/SnOr.cc
  src/fsim/fsim3/SnXor.cc
  )

set (graphsat_SOURCES
  src/graphsat/AssignList.cc
  src/graphsat/GraphSat.cc
  src/graphsat/GraphSatR.cc
  src/graphsat/SaBase.cc
  src/graphsat/SaFactory.cc
  src/graphsat/SaUIP1.cc
  src/graphsat/SaUIP2.cc
  src/graphsat/SatClause.cc
  src/graphsat/VarHeap.cc
  )

set (minpat_SOURCES
  src/minpat/KDet2.cc
  src/minpat/KDet2Op.cc
  src/minpat/Verifier.cc
  src/minpat/GcNodeHeap.cc
  src/minpat/GcNodeHeap2.cc
  src/minpat/GcSolver.cc
  src/minpat/GcSolver2.cc
  src/minpat/MinPatBase.cc
  src/minpat/MinPatNaive.cc
  src/minpat/MinPatSimple.cc
  src/minpat/MinPatDsatur.cc
  src/minpat/MinPatDsatur2.cc
  src/minpat/FaultAnalyzer.cc
  src/minpat/FgMgr.cc
  src/minpat/TpgCnf0.cc
  src/minpat/TpgCnf1.cc
  src/minpat/TpgCnf2.cc
  )

set (rtpg_SOURCES
  src/rtpg/FopRtpg.cc
  src/rtpg/RtpgImpl.cc
  )

set (atpg_tclsh_SOURCES
  src/atpg_tclsh/tclAppInit.cc
  src/atpg_tclsh/atpg_init.cc
  src/atpg_tclsh/AtpgCmd.cc
  src/atpg_tclsh/AtpgMgr.cc
  src/atpg_tclsh/AtpgMsgHandler.cc
  src/atpg_tclsh/NtwkIoCmd.cc
  src/atpg_tclsh/PrintFaultCmd.cc
  src/atpg_tclsh/PrintPatCmd.cc
  src/atpg_tclsh/PrintPatStatsCmd.cc
  src/atpg_tclsh/PrintStatsCmd.cc
  src/atpg_tclsh/TimeCmd.cc
  src/atpg_tclsh/RtpgCmd.cc
  src/atpg_tclsh/DtpgCmd.cc
  src/atpg_tclsh/MinPatCmd.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(satpg_base OBJECT ${base_SOURCES})

add_library(satpg_dtpg OBJECT ${dtpg_SOURCES})

add_library(satpg_fsim OBJECT ${fsim_SOURCES})

#add_library(satpg_graphsat OBJECT ${graphsat_SOURCES})

add_library(satpg_minpat OBJECT ${minpat_SOURCES})

add_library(satpg_rtpg OBJECT ${rtpg_SOURCES})

if (TCL_FOUND)
  add_executable(atpg_tclsh
    ${atpg_tclsh_SOURCES}
    $<TARGET_OBJECTS:satpg_dtpg>
#    $<TARGET_OBJECTS:satpg_graphsat>
    $<TARGET_OBJECTS:satpg_fsim>
    $<TARGET_OBJECTS:satpg_minpat>
    $<TARGET_OBJECTS:satpg_rtpg>
    $<TARGET_OBJECTS:satpg_base>
    )

  target_link_libraries(atpg_tclsh
    ym_networks
    ym_tclpp
    ${TCL_LIBRARIES}
    )
endif (TCL_FOUND)


# ===================================================================
#  テストの設定
# ===================================================================

add_executable ( satpg_simnode_test
  src/fsim/fsim3/simnode_test.cc
  $<TARGET_OBJECTS:satpg_fsim>
  $<TARGET_OBJECTS:satpg_base>
  )

target_link_libraries( satpg_simnode_test
    ym_networks
    )

add_test (
  NAME satpg_simnode_test_run
  COMMAND satpg_simnode_test
  )