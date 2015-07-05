
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ${TCL_INCLUDE_PATH}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(ym_tclpp
  src/StrBuf.h
  src/TclBase.cc
  src/TclCmd.cc
  src/TclHandler.cc
  src/TclObj.cc
  src/TclObjMsgHandler.cc
  src/TclPopt.cc
  src/TclPoptGroup.h
  src/TclVarTrace.cc
  )

target_link_libraries(ym_tclpp
  ${TCL_LIBRARY}
  )


# ===================================================================
#  インストールターゲットの設定
# ===================================================================

install(
  TARGETS ym_tclpp
  DESTINATION lib
  EXPORT ymtools-targets
  )

install(
  DIRECTORY YmTclpp
  DESTINATION include
  )
