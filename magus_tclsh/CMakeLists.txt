
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  include
  ${TCL_INCLUDE_PATH}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
set (bnet_SOURCES
  bnet/BNetCmd.cc
#  bnet/BNetElimCmd.cc
  bnet/BNetInfoCmd.cc
#  bnet/BNetIoCmd.cc
#  bnet/BNetNdCmd.cc
#  bnet/BNetNodeCmd.cc
  bnet/bnet_init.cc
  )

set (core_SOURCES
  core/BNetHandle.cc
  core/ChgNetwork.cc
  core/ClrNetwork.cc
  core/CopyNetwork.cc
  core/CurNetwork.cc
  core/DelNetwork.cc
  core/DumpLibrary.cc
  core/ForNetwork.cc
  core/ListNetwork.cc
  core/MagCmd.cc
  core/MagMgr.cc
  core/MvnHandle.cc
  core/NetCmd.cc
  core/NetHandle.cc
  core/NewBNetwork.cc
  core/NewMvn.cc
  core/PopNetwork.cc
  core/PushNetwork.cc
  core/ReadBlif.cc
  core/ReadIscas89.cc
  core/ReadLiberty.cc
  core/ReadLibrary.cc
  core/ReadMislib.cc
  core/RestoreLibrary.cc
  core/WriteBlif.cc
  core/WriteVerilog.cc
  core/core_init.cc
  )

set (equiv_SOURCES
  equiv/EquivCmd.cc
  )

set (lutmap_SOURCES
  lutmap/AreaMapCmd.cc
  lutmap/Conv2BNetCmd.cc
  lutmap/DelayMapCmd.cc
  lutmap/DumpLutCmd.cc
  lutmap/LutmapCmd.cc
  lutmap/WriteVqmCmd.cc
  )

set (misc_SOURCES
  misc/TimeCmd.cc
  misc/misc_init.cc
  )

set (mvn_SOURCES
  mvn/MvnCmd.cc
  mvn/MvnReadVerilog.cc
  mvn/mvn_init.cc
  )

set (seal_SOURCES
  seal/DupCmd.cc
  seal/McaCmd.cc
  seal/McaCmd2.cc
  seal/SealCmd.cc
  seal/TMRCmd.cc
  seal/seal_init.cc
  )

set (simplify_SOURCES
  simplify/LrwCmd.cc
  simplify/simplify_init.cc
  )

set (techmap_SOURCES
  techmap/AreaMapCmd.cc
  techmap/TechmapCmd.cc
  )

set (magus_SOURCES
  magus_init.cc
  tclAppInit.cc
  )


# ===================================================================
#  release ターゲットの設定
# ===================================================================
add_executable (magus_tclsh
  ${magus_SOURCES}
  ${bnet_SOURCES}
  ${core_SOURCES}
  ${equiv_SOURCES}
  ${lutmap_SOURCES}
  ${misc_SOURCES}
  ${mvn_SOURCES}
#  ${seal_SOURCES}
  ${simplify_SOURCES}
#  ${techmap_SOURCES}
  $<TARGET_OBJECTS:magus_cequiv_a>
  $<TARGET_OBJECTS:magus_sbjgraph_a>
#  $<TARGET_OBJECTS:magus_seal_a>
  $<TARGET_OBJECTS:magus_lutmap_a>
#  $<TARGET_OBJECTS:magus_cellmap_a>
  $<TARGET_OBJECTS:magus_sbjgraph_a>
  $<TARGET_OBJECTS:ym_common_a>
  $<TARGET_OBJECTS:ym_logic_a>
  $<TARGET_OBJECTS:ym_cell_a>
  $<TARGET_OBJECTS:ym_bnet_a>
  $<TARGET_OBJECTS:ym_sat_a>
  $<TARGET_OBJECTS:ym_mincov_a>
  $<TARGET_OBJECTS:ym_mvn_a>
  $<TARGET_OBJECTS:ym_sat_a>
  $<TARGET_OBJECTS:ym_udgraph_a>
  $<TARGET_OBJECTS:ym_verilog_a>
  $<TARGET_OBJECTS:ym_tclpp_a>
  )

target_compile_options (magus_tclsh
  PRIVATE "-O3"
  )

target_compile_definitions (magus_tclsh
  PRIVATE "-DNDEBUG"
  )

target_link_libraries (magus_tclsh
  ${YM_LIB_DEPENDS}
  ${TCL_LIBRARY}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( magus_tclsh
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================
add_executable (magus_tclsh_p
  ${magus_SOURCES}
#  ${bdn_SOURCES}
  ${bnet_SOURCES}
  ${core_SOURCES}
  ${equiv_SOURCES}
  ${lutmap_SOURCES}
  ${misc_SOURCES}
  ${mvn_SOURCES}
#  ${seal_SOURCES}
  ${simplify_SOURCES}
#  ${techmap_SOURCES}
  $<TARGET_OBJECTS:magus_cequiv_p>
  $<TARGET_OBJECTS:magus_sbjgraph_p>
#  $<TARGET_OBJECTS:magus_seal_p>
  $<TARGET_OBJECTS:magus_lutmap_p>
#  $<TARGET_OBJECTS:magus_cellmap_p>
  $<TARGET_OBJECTS:magus_sbjgraph_p>
  $<TARGET_OBJECTS:ym_common_p>
  $<TARGET_OBJECTS:ym_logic_p>
  $<TARGET_OBJECTS:ym_cell_p>
  $<TARGET_OBJECTS:ym_bnet_p>
  $<TARGET_OBJECTS:ym_sat_p>
  $<TARGET_OBJECTS:ym_mincov_p>
  $<TARGET_OBJECTS:ym_mvn_p>
  $<TARGET_OBJECTS:ym_sat_p>
  $<TARGET_OBJECTS:ym_udgraph_p>
  $<TARGET_OBJECTS:ym_verilog_p>
  $<TARGET_OBJECTS:ym_tclpp_p>
  )

target_compile_options (magus_tclsh_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (magus_tclsh_p
  PRIVATE "-DNDEBUG"
  )

target_link_libraries (magus_tclsh_p
  ${YM_LIB_DEPENDS}
  ${TCL_LIBRARY}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( magus_tclsh_p
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  debug ターゲットの設定
# ===================================================================
add_executable (magus_tclsh_d
  ${magus_SOURCES}
#  ${bdn_SOURCES}
  ${bnet_SOURCES}
  ${core_SOURCES}
  ${equiv_SOURCES}
  ${lutmap_SOURCES}
  ${misc_SOURCES}
  ${mvn_SOURCES}
#  ${seal_SOURCES}
  ${simplify_SOURCES}
#  ${techmap_SOURCES}
  $<TARGET_OBJECTS:magus_cequiv_d>
  $<TARGET_OBJECTS:magus_sbjgraph_d>
#  $<TARGET_OBJECTS:magus_seal_d>
  $<TARGET_OBJECTS:magus_lutmap_d>
#  $<TARGET_OBJECTS:magus_cellmap_d>
  $<TARGET_OBJECTS:magus_sbjgraph_d>
  $<TARGET_OBJECTS:ym_common_d>
  $<TARGET_OBJECTS:ym_logic_d>
  $<TARGET_OBJECTS:ym_cell_d>
  $<TARGET_OBJECTS:ym_bnet_d>
  $<TARGET_OBJECTS:ym_sat_d>
  $<TARGET_OBJECTS:ym_mincov_d>
  $<TARGET_OBJECTS:ym_mvn_d>
  $<TARGET_OBJECTS:ym_sat_d>
  $<TARGET_OBJECTS:ym_udgraph_d>
  $<TARGET_OBJECTS:ym_verilog_d>
  $<TARGET_OBJECTS:ym_tclpp_d>
  )

target_compile_options (magus_tclsh_d
  PRIVATE "-g"
  )

target_link_libraries (magus_tclsh_d
  ${YM_LIB_DEPENDS}
  ${TCL_LIBRARY}
  )

if ( GPERFTOOLS_FOUND )
  target_link_libraries ( magus_tclsh_d
    ${GPERFTOOLS_LIBRARIES}
    )
endif ()


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

install (
  TARGETS magus_tclsh magus_tclsh_p magus_tclsh_d
  DESTINATION bin
  )
