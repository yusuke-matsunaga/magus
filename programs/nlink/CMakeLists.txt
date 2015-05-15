
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

set (nlink_SOURCES
  src/nlink.cc
  src/NlProblem.cc
  src/NlScanner.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
#add_library(satpg_base OBJECT ${base_SOURCES})

#add_library(satpg_dtpg OBJECT ${dtpg_SOURCES})

#add_library(satpg_fsim OBJECT ${fsim_SOURCES})

#add_library(satpg_graphsat OBJECT ${graphsat_SOURCES})

#add_library(satpg_minpat OBJECT ${minpat_SOURCES})

#add_library(satpg_rtpg OBJECT ${rtpg_SOURCES})

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
  )

target_link_libraries(nlink
  ym_logic
  )

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