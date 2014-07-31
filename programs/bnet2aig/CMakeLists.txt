
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  ${GMP_INCLUDE_DIR}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_executable(bnet2aig
  bnet2aig.cc
  )

target_link_libraries(bnet2aig
  ym_networks
  ym_cell
  ym_logic
  ym_utils
  )
