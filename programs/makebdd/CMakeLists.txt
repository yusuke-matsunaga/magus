
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
add_executable(makebdd
  makebdd.cc
  )

target_link_libraries(makebdd
  ym_networks
  ym_cell
  ym_logic
  ym_utils
  )
