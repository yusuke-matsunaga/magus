
# ===================================================================
# サブディレクトリの設定
# ===================================================================
add_subdirectory (src/base)
add_subdirectory (src/dtpg)
add_subdirectory (src/fsim)
add_subdirectory (src/main)
add_subdirectory (src/minpat)
add_subdirectory (src/rtpg)

#if (TCL_FOUND)
#  add_subdirectory (atpg_tclsh)
#endif (TCL_FOUND)
