
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
  src/compiler/ObjHandle.cc
  src/compiler/YmslArrayRef.cc
  src/compiler/YmslArrayType.cc
  src/compiler/YmslCompiler.cc
  src/compiler/YmslEnumType.cc
  src/compiler/YmslExpr.cc
  src/compiler/YmslFalse.cc
  src/compiler/YmslFloatConst.cc
  src/compiler/YmslFuncCall.cc
  src/compiler/YmslFunction.cc
  src/compiler/YmslIntConst.cc
  src/compiler/YmslLabel.cc
  src/compiler/YmslMapType.cc
  src/compiler/YmslMemberRef.cc
  src/compiler/YmslNamedType.cc
  src/compiler/YmslOpExpr.cc
  src/compiler/YmslPrimType.cc
  src/compiler/YmslScope.cc
  src/compiler/YmslSetType.cc
  src/compiler/YmslStringConst.cc
  src/compiler/YmslTrue.cc
  src/compiler/YmslType.cc
  src/compiler/YmslTypeMgr.cc
  src/compiler/YmslVar.cc
  src/compiler/YmslVarExpr.cc
  src/engine/YmslVM.cc
  src/parser/Ast.cc
  src/parser/AstArrayRef.cc
  src/parser/AstAssignment.cc
  src/parser/AstBinOp.cc
  src/parser/AstBlockStmt.cc
  src/parser/AstBreak.cc
  src/parser/AstContinue.cc
  src/parser/AstDoWhile.cc
  src/parser/AstEnumDecl.cc
  src/parser/AstExpr.cc
  src/parser/AstExprStmt.cc
  src/parser/AstFalse.cc
  src/parser/AstFloatConst.cc
  src/parser/AstFor.cc
  src/parser/AstFuncCall.cc
  src/parser/AstFuncDecl.cc
  src/parser/AstGoto.cc
  src/parser/AstIf.cc
  src/parser/AstImport.cc
  src/parser/AstIteOp.cc
  src/parser/AstIntConst.cc
  src/parser/AstLabel.cc
  src/parser/AstMemberRef.cc
  src/parser/AstMgr.cc
  src/parser/AstParam.cc
  src/parser/AstPrinter.cc
  src/parser/AstReturn.cc
  src/parser/AstStatement.cc
  src/parser/AstStringConst.cc
  src/parser/AstSwitch.cc
  src/parser/AstSymbolExpr.cc
  src/parser/AstToplevel.cc
  src/parser/AstTrue.cc
  src/parser/AstType.cc
  src/parser/AstUniOp.cc
  src/parser/AstVarDecl.cc
  src/parser/AstWhile.cc
  src/parser/RsrvWordDic.cc
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
