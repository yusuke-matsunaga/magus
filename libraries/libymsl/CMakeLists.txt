
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
  src/parser/RsrvWordDic.cc
  src/parser/YmslScanner.cc

  ${CMAKE_CURRENT_BINARY_DIR}/grammer.cc

  src/ast/Ast.cc
  src/ast/AstArrayRef.cc
  src/ast/AstAssignment.cc
  src/ast/AstBinOp.cc
  src/ast/AstBlockStmt.cc
  src/ast/AstBreak.cc
  src/ast/AstCaseItem.cc
  src/ast/AstContinue.cc
  src/ast/AstDoWhile.cc
  src/ast/AstEnumConst.cc
  src/ast/AstEnumDecl.cc
  src/ast/AstExpr.cc
  src/ast/AstExprStmt.cc
  src/ast/AstFalse.cc
  src/ast/AstFloatConst.cc
  src/ast/AstFor.cc
  src/ast/AstFuncCall.cc
  src/ast/AstFuncDecl.cc
  src/ast/AstGoto.cc
  src/ast/AstIf.cc
  src/ast/AstImport.cc
  src/ast/AstIteOp.cc
  src/ast/AstIntConst.cc
  src/ast/AstLabel.cc
  src/ast/AstMemberRef.cc
  src/ast/AstMgr.cc
  src/ast/AstModule.cc
  src/ast/AstParam.cc
  src/ast/AstPrinter.cc
  src/ast/AstReturn.cc
  src/ast/AstStatement.cc
  src/ast/AstStringConst.cc
  src/ast/AstSwitch.cc
  src/ast/AstSymbol.cc
  src/ast/AstSymbolExpr.cc
  src/ast/AstToplevel.cc
  src/ast/AstTrue.cc
  src/ast/AstType.cc
  src/ast/AstUniOp.cc
  src/ast/AstVarDecl.cc
  src/ast/AstWhile.cc

  src/ir/IrArrayRef.cc
  src/ir/IrArrayType.cc
  src/ir/IrEnumExpr.cc
  src/ir/IrEnumType.cc
  src/ir/IrExpr.cc
  src/ir/IrFalse.cc
  src/ir/IrFloatConst.cc
  src/ir/IrFuncCall.cc
  src/ir/IrFuncExpr.cc
  src/ir/IrFunction.cc
  src/ir/IrHandle.cc
  src/ir/IrIntConst.cc
  src/ir/IrMapType.cc
  src/ir/IrMemberRef.cc
  src/ir/IrMgr.cc
  src/ir/IrNamedType.cc
  src/ir/IrOpExpr.cc
  src/ir/IrPrimType.cc
  src/ir/IrScope.cc
  src/ir/IrScopeExpr.cc
  src/ir/IrSetType.cc
  src/ir/IrStringConst.cc
  src/ir/IrTrue.cc
  src/ir/IrType.cc
  src/ir/IrTypeMgr.cc
  src/ir/IrVar.cc
  src/ir/IrVarExpr.cc

  src/compiler/YmslCompiler.cc
  src/compiler/YmslLabel.cc

  src/engine/YmslVM.cc

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
