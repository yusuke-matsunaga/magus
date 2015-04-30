
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ${Readline_INCLUDE_DIR}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (ymsh_SOURCES
  src/AliasCmd.cc
  src/DirCmd.cc
  src/ExitCmd.cc
  src/SetCmd.cc
  src/Ymsh.cc
  src/YmshCmd.cc
  src/YmshCmdTable.cc
  src/YmshImpl.cc
  src/YmshObjTable.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_library(ym_ymsh
  ${ymsh_SOURCES}
  )

target_link_libraries(ym_ymsh
  ${Readline_LIBRARY}
  )
