
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  )


# ===================================================================
# サブディレクトリの設定
# ===================================================================

add_subdirectory (c++-src/equiv)
add_subdirectory (c++-src/sbjgraph)
add_subdirectory (c++-src/lutmap)
add_subdirectory (c++-src/cellmap)
