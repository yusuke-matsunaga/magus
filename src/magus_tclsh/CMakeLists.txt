
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  include
  ${TCL_INCLUDE_PATH}
  ${YmTclpp_INCLUDE_DIRS}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================
set (bdn_SOURCES
  bdn/BdnCmd.cc
  )

set (bnet_SOURCES
  bnet/BNetCmd.cc
  bnet/BNetElimCmd.cc
  bnet/BNetInfoCmd.cc
  bnet/BNetIoCmd.cc
  bnet/BNetNdCmd.cc
  bnet/BNetNodeCmd.cc
  bnet/bnet_init.cc
  )

set (core_SOURCES
  core/BNetHandle.cc
  core/BdnHandle.cc
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
  core/NewBdn.cc
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
  lutmap/Ln2BNet.cc
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
  techmap/DumpCnCmd.cc
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
  ${bdn_SOURCES}
  ${bnet_SOURCES}
  ${core_SOURCES}
  ${equiv_SOURCES}
  ${lutmap_SOURCES}
  ${misc_SOURCES}
  ${mvn_SOURCES}
  ${seal_SOURCES}
  ${simplify_SOURCES}
  ${techmap_SOURCES}
  )

target_compile_options (magus_tclsh
  PRIVATE "-O3"
  )

target_compile_definitions (magus_tclsh
  PRIVATE "-DNDEBUG"
  )

target_link_libraries (magus_tclsh
  magus_cequiv
  magus_seal
  magus_techmap
  ym_networks
  ym_cell
  ym_verilog
  ym_logic
  ym_tclpp
  ym_utils
  )


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================
add_executable (magus_tclsh_p
  ${magus_SOURCES}
  ${bdn_SOURCES}
  ${bnet_SOURCES}
  ${core_SOURCES}
  ${equiv_SOURCES}
  ${lutmap_SOURCES}
  ${misc_SOURCES}
  ${mvn_SOURCES}
  ${seal_SOURCES}
  ${simplify_SOURCES}
  ${techmap_SOURCES}
  )

target_compile_options (magus_tclsh_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (magus_tclsh_p
  PRIVATE "-DNDEBUG"
  )

target_link_libraries (magus_tclsh_p
  magus_cequiv_p
  magus_seal_p
  magus_techmap_p
  ym_networks_p
  ym_cell_p
  ym_verilog_p
  ym_logic_p
  ym_tclpp_p
  ym_utils_p
  )


# ===================================================================
#  debug ターゲットの設定
# ===================================================================
add_executable (magus_tclsh_d
  ${magus_SOURCES}
  ${bdn_SOURCES}
  ${bnet_SOURCES}
  ${core_SOURCES}
  ${equiv_SOURCES}
  ${lutmap_SOURCES}
  ${misc_SOURCES}
  ${mvn_SOURCES}
  ${seal_SOURCES}
  ${simplify_SOURCES}
  ${techmap_SOURCES}
  )

target_compile_options (magus_tclsh_d
  PRIVATE "-g"
  )

target_link_libraries (magus_tclsh_d
  magus_cequiv_d
  magus_seal_d
  magus_techmap_d
  ym_networks_d
  ym_cell_d
  ym_verilog_d
  ym_logic_d
  ym_tclpp_d
  ym_utils_d
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

install (
  TARGETS magus_tclsh magus_tclsh_p magus_tclsh_d
  DESTINATION bin
  )
