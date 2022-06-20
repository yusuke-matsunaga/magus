
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_BINARY_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${YM_SUBMODULE_INCLUDE_LIST}
  ${PROJECT_SOURCE_DIR}/ym-tclpp/include
  ${TCL_INCLUDE_PATH}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
set (bnet_SOURCES
  bnet/BNetCmd.cc
  bnet/BNetInfoCmd.cc
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

set ( lutmap_SOURCES
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
  ${techmap_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj>
  $<TARGET_OBJECTS:magus_sbjgraph_obj>
#  $<TARGET_OBJECTS:magus_seal>
  $<TARGET_OBJECTS:magus_lutmap_obj>
  $<TARGET_OBJECTS:magus_cellmap_obj>
  $<TARGET_OBJECTS:magus_sbjgraph_obj>
  $<TARGET_OBJECTS:ym_tclpp_obj>
  ${YM_SUBMODULE_OBJ_LIST}
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
  ${techmap_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_p>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_p>
#  $<TARGET_OBJECTS:magus_seal_obj_p>
  $<TARGET_OBJECTS:magus_lutmap_obj_p>
  $<TARGET_OBJECTS:magus_cellmap_obj_p>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_p>
  $<TARGET_OBJECTS:ym_tclpp_obj_p>
  ${YM_SUBMODULE_OBJ_P_LIST}
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
  ${techmap_SOURCES}
  $<TARGET_OBJECTS:magus_equiv_obj_d>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
#  $<TARGET_OBJECTS:magus_seal_d>
  $<TARGET_OBJECTS:magus_lutmap_obj_d>
  $<TARGET_OBJECTS:magus_cellmap_obj_d>
  $<TARGET_OBJECTS:magus_sbjgraph_obj_d>
  $<TARGET_OBJECTS:ym_tclpp_obj_d>
  ${YM_SUBMODULE_OBJ_D_LIST}
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
