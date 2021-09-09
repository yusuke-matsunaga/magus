

# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories (
  )


# ===================================================================
#  マクロの定義
# ===================================================================


# ===================================================================
# サブディレクトリの設定
# ===================================================================


# ===================================================================
#  テストの設定
# ===================================================================

set ( RUN_PYM_BNET
  env PYTHONPATH=${PROJECT_BINARY_DIR}/magus_core/debug
  ${Python3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/magus_core_test.py)

add_test ( PyReadBlifTest
  ${RUN_PYM_BNET} ReadBlifTest
  )

set_property ( TEST PyReadBlifTest
  PROPERTY ENVIRONMENT TESTDATA_DIR=${PROJECT_SOURCE_DIR}/magus_core/testdata
  )

add_test ( PyReadIscas89Test
  ${RUN_PYM_BNET} ReadIscas89Test
  )

set_property ( TEST PyReadIscas89Test
  PROPERTY ENVIRONMENT TESTDATA_DIR=${PROJECT_SOURCE_DIR}/magus_core/testdata
  )

add_test ( PyEquivTest
  ${RUN_PYM_BNET} EquivTest
  )

set_property ( TEST PyEquivTest
  PROPERTY ENVIRONMENT TESTDATA_DIR=${PROJECT_SOURCE_DIR}/magus_core/testdata
  )
