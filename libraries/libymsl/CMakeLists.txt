
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (ymsl_SOURCES
  src/RsrvWordDic.cc
  src/YmslAstBinOp.cc
  src/YmslAstFloat.cc
  src/YmslAstImpl.cc
  src/YmslAstInt.cc
  src/YmslAstString.cc
  src/YmslAstSymbol.cc
  src/YmslAstUniOp.cc
  src/YmslParser.cc
  src/YmslScanner.cc

  ${CMAKE_CURRENT_BINARY_DIR}/grammer.cc
  )


# Create target for the parser
add_custom_target ( grammer ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  )

# Create custom command for bison/yacc (note the DEPENDS)
add_custom_command (
  COMMAND "${BISON_EXECUTABLE}"
  -y "${CMAKE_CURRENT_SOURCE_DIR}/src/grammer.yy"
  -o "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  -d -v
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.output"
  )

# Since verilog_grammer.cc does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties ( ${CMAKE_CURRENT_BINARY_DIR}/grammer.cc GENERATED )


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_library(ymsl SHARED
  ${ymsl_SOURCES}
  )

target_link_libraries(ymsl
  ${Readline_LIBRARY}
  ym_utils
  )

add_executable(scanner_test
  tests/scanner_test.cc
  )

target_link_libraries(scanner_test
  ymsl
  )