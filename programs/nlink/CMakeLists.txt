
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

set (common_SOURCES
  src/common/NlGraph.cc
  src/common/NlProblem.cc
  src/common/NlSolution.cc
  src/common/NlScanner.cc
  )

set (graphsat_SOURCES
  src/graphsat/GraphSat.cc
  src/graphsat/GraphSatImpl.cc
  src/graphsat/GraphSatR.cc
  src/graphsat/AssignList.cc
  src/graphsat/SaBase.cc
  src/graphsat/SaFactory.cc
  src/graphsat/SaUIP1.cc
  src/graphsat/SaUIP2.cc
  src/graphsat/SatClause.cc
  src/graphsat/VarHeap.cc
  src/graphsat/GsGraph.cc
  )

set (solver_SOURCES
  src/solver/NlSolverGs.cc
  src/solver/NlSolver0.cc
  src/solver/NpGraph.cc
  src/solver/NlSolver1.cc
  src/solver/NlSolver2.cc
  src/solver/NlSolver2_5.cc
  src/solver/NlSolver3.cc
  src/solver/NlSolverFr.cc
  src/solver/FrontierInfo.cc
  src/solver/FrNode.cc
  )

set (nlink_SOURCES
  src/nlink.cc
  )

set (nlink2_SOURCES
  src/nlink2.cc
  )

set (nlink3_SOURCES
  src/nlink3.cc
  )

set (nlview_SOURCES
  src/gui/NlView.cc
  src/gui/nlview.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library (nlink_common
  OBJECT
  ${common_SOURCES}
  ${graphsat_SOURCES}
  ${solver_SOURCES}
  )

#if (TCL_FOUND)
#  add_executable(nlink
#    ${nlink_SOURCES}
#    $<TARGET_OBJECTS:satpg_dtpg>
#    $<TARGET_OBJECTS:satpg_graphsat>
#    $<TARGET_OBJECTS:satpg_fsim>
#    $<TARGET_OBJECTS:satpg_minpat>
#    $<TARGET_OBJECTS:satpg_rtpg>
#    $<TARGET_OBJECTS:satpg_base>
#    )
#
#  target_link_libraries(nlink
#    ym_logic
#    ym_tclpp
#    ${TCL_LIBRARIES}
#    )
#endif (TCL_FOUND)

add_executable(nlink
  ${nlink_SOURCES}
  $<TARGET_OBJECTS:nlink_common>
  )

target_link_libraries(nlink
  ym_logic
  )

if (Qt4_FOUND)
  QT4_WRAP_CPP(HEADERS_MOC
    src/gui/NlView.h
    )

  add_executable(nlview
    ${nlview_SOURCES}
    ${HEADERS_MOC}
    $<TARGET_OBJECTS:nlink_common>
    )

  target_link_libraries(nlview
    ym_logic
    ym_utils
    ${QT_LIBRARIES}
    )

endif (Qt4_FOUND)


# ===================================================================
#  テストの設定
# ===================================================================

#add_executable ( satpg_simnode_test
#  src/fsim/fsim3/simnode_test.cc
#  $<TARGET_OBJECTS:satpg_fsim>
#  $<TARGET_OBJECTS:satpg_base>
#  )

#target_link_libraries( satpg_simnode_test
#    ym_networks
#    )

#add_test (
#  NAME satpg_simnode_test_run
#  COMMAND satpg_simnode_test
#  )