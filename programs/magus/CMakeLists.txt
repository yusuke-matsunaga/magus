
# ===================================================================
# サブディレクトリの設定
# ===================================================================
add_subdirectory (src/cequiv)
#add_subdirectory (src/seal)
add_subdirectory (src/techmap)

if (TCL_FOUND)
  add_subdirectory (src/magus_tclsh)
endif (TCL_FOUND)
