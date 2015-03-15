
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

  src/dtpg/driver/DtpgDriverImpl.cc

  src/dtpg/engine/LitMap.cc
  src/dtpg/engine/SatEngine.cc
  src/dtpg/engine/SatEngineMulti.cc
  src/dtpg/engine/SatEngineMulti2.cc
  src/dtpg/engine/SatEngineSingle.cc
  src/dtpg/engine/SatEngineSingle2.cc
  src/dtpg/engine/SmtEngine.cc
  src/dtpg/engine/SmtEngineSingle.cc

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
  src/minpat/FopRofsim.cc
  src/minpat/KDet.cc
  src/minpat/KDetOp.cc
  src/minpat/MinPatImpl.cc
  src/minpat/Verifier.cc
  src/minpat/GreedyMinPat.cc
  src/minpat/MaxCompat.cc
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
add_library(satpg_base STATIC ${base_SOURCES})

add_library(satpg_dtpg STATIC ${dtpg_SOURCES})

add_library(satpg_fsim STATIC ${fsim_SOURCES})

add_library(satpg_graphsat STATIC ${graphsat_SOURCES})

add_library(satpg_minpat STATIC ${minpat_SOURCES})

add_library(satpg_rtpg STATIC ${rtpg_SOURCES})

if (TCL_FOUND)
  add_executable(atpg_tclsh
    ${atpg_tclsh_SOURCES}
    )

  target_link_libraries(atpg_tclsh
    satpg_dtpg
    satpg_graphsat
    satpg_fsim
    satpg_minpat
    satpg_rtpg
    satpg_base
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
  )

target_link_libraries( satpg_simnode_test
  satpg_fsim
  )

add_test (
  NAME satpg_simnode_test_run
  COMMAND satpg_simnode_test
  )