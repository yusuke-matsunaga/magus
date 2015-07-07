
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (SOURCES
  CecMgr.cc
  FraigMgr.cc
  FraigMgrImpl.cc
  FraigNode.cc
  check_ceq.cc
  check_ceq2.cc
  make_miter.cc
  )


# ===================================================================
#  release ターゲットの設定
# ===================================================================

add_library (magus_cequiv
  STATIC
  ${SOURCES}
  )

target_compile_options (magus_cequiv
  PRIVATE "-O3"
  )

target_compile_definitions (magus_cequiv
  PRIVATE "-DNDEBUG"
  )


# ===================================================================
#  release with debug info ターゲットの設定
# ===================================================================

add_library (magus_cequiv_p
  STATIC
  ${SOURCES}
  )

target_compile_options (magus_cequiv_p
  PRIVATE "-O2" "-g"
  )

target_compile_definitions (magus_cequiv_p
  PRIVATE "-DNDEBUG"
  )


# ===================================================================
#  debug ターゲットの設定
# ===================================================================

add_library (magus_cequiv_d
  STATIC
  ${SOURCES}
  )

target_compile_options (magus_cequiv_d
  PRIVATE "-g"
  )
