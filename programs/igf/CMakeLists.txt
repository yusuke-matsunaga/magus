
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  include
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (igugen_SOURCES
  src/igugen/igugen.cc
  src/igugen/VarHeap.cc
  )

set (libigf_SOURCES
  src/libigf/IGU.cc
  src/libigf/IguGen.cc
  src/libigf/PhfGraph.cc
  src/libigf/RandHashGen.cc
  src/libigf/RvMgr.cc
  src/libigf/VarFunc.cc
  src/libigf/Variable.cc
  src/libigf/XorFunc.cc
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(libigf STATIC ${libigf_SOURCES})

add_executable(igugen
  ${igugen_SOURCES}
  )

target_link_libraries(igugen
  libigf
  ym_utils
  )


# ===================================================================
#  テストの設定
# ===================================================================
