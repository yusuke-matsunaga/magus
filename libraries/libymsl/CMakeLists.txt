
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (ymsl_SOURCES
  src/YmslExecutable.cc
  src/YmslFunc.cc
  src/YmslValueType.cc
  src/YmslVar.cc
  src/parser/Ast.cc
  src/parser/AstBlock.cc
  src/parser/AstDeclBase.cc
  src/parser/AstExpr.cc
  src/parser/AstStatement.cc
  src/parser/AstValueType.cc
  src/parser/RsrvWordDic.cc
  src/parser/YmslDict.cc
  src/parser/Driver.cc
  src/parser/YmslScanner.cc

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
  -y "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/grammer.yy"
  -o "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  -d -v
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/grammer.cc"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.hh"
  "${CMAKE_CURRENT_BINARY_DIR}/grammer.output"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/parser/grammer.yy"
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

add_executable(parser_test
  tests/parser_test.cc
  )

target_link_libraries(parser_test
  ymsl
  )
