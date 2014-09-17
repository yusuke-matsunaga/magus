
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  include
  ${CMAKE_CURRENT_BINARY_DIR}
  )


# ===================================================================
#  ソースファイルの設定
# ===================================================================

set (common_SOURCES
  src/common/BitVector.cc
  src/common/VlMgr.cc
  src/common/VlUdpVal.cc
  src/common/VlValue.cc
  )

set (elaborator_SOURCES
  src/elaborator/ei/EiAssignment.cc
  src/elaborator/ei/EiAttribute.cc
  src/elaborator/ei/EiBinaryOp.cc
  src/elaborator/ei/EiBitSelect.cc
  src/elaborator/ei/EiBlockScope.cc
  src/elaborator/ei/EiBlockStmt.cc
  src/elaborator/ei/EiConcatOp.cc
  src/elaborator/ei/EiCondStmt.cc
  src/elaborator/ei/EiConstant.cc
  src/elaborator/ei/EiContAssign.cc
  src/elaborator/ei/EiControl.cc
  src/elaborator/ei/EiDecl.cc
  src/elaborator/ei/EiDeclArray.cc
  src/elaborator/ei/EiDeclHead.cc
  src/elaborator/ei/EiDelay.cc
  src/elaborator/ei/EiExpr.cc
  src/elaborator/ei/EiFactory.cc
  src/elaborator/ei/EiFuncCall.cc
  src/elaborator/ei/EiGenBlock.cc
  src/elaborator/ei/EiGenvar.cc
  src/elaborator/ei/EiIODecl.cc
  src/elaborator/ei/EiImpNet.cc
  src/elaborator/ei/EiLhs.cc
  src/elaborator/ei/EiMiscStmt.cc
  src/elaborator/ei/EiModule.cc
  src/elaborator/ei/EiParamAssign.cc
  src/elaborator/ei/EiParameter.cc
  src/elaborator/ei/EiPartSelect.cc
  src/elaborator/ei/EiPort.cc
  src/elaborator/ei/EiPrimary.cc
  src/elaborator/ei/EiPrimitive.cc
  src/elaborator/ei/EiProcess.cc
  src/elaborator/ei/EiRange.cc
  src/elaborator/ei/EiStmt.cc
  src/elaborator/ei/EiTaskFunc.cc
  src/elaborator/ei/EiTernaryOp.cc
  src/elaborator/ei/EiToplevel.cc
  src/elaborator/ei/EiUdp.cc
  src/elaborator/ei/EiUnaryOp.cc

  src/elaborator/elb_mgr/AttrHash.cc
  src/elaborator/elb_mgr/ElbContAssign.cc
  src/elaborator/elb_mgr/ElbDecl.cc
  src/elaborator/elb_mgr/ElbExpr.cc
  src/elaborator/elb_mgr/ElbMgr.cc
  src/elaborator/elb_mgr/ElbParamAssign.cc
  src/elaborator/elb_mgr/ElbParameter.cc
  src/elaborator/elb_mgr/ElbPrimitive.cc
  src/elaborator/elb_mgr/ElbProcess.cc
  src/elaborator/elb_mgr/ElbScope.cc
  src/elaborator/elb_mgr/ElbStmt.cc
  src/elaborator/elb_mgr/ElbUserSystf.cc
  src/elaborator/elb_mgr/ModuleHash.cc
  src/elaborator/elb_mgr/ObjDict.cc
  src/elaborator/elb_mgr/TagDict.cc
  src/elaborator/elb_mgr/VlNamedObj.cc

  src/elaborator/main/AttrDict.cc
  src/elaborator/main/AttrGen.cc
  src/elaborator/main/CfDict.cc
  src/elaborator/main/DeclGen.cc
  src/elaborator/main/Elaborator.cc
  src/elaborator/main/ElbEnv.cc
  src/elaborator/main/ElbProxy.cc
  src/elaborator/main/ExprGen_error.cc
  src/elaborator/main/ExprGen_funccall.cc
  src/elaborator/main/ExprGen_main.cc
  src/elaborator/main/ExprGen_operation.cc
  src/elaborator/main/ExprGen_primary.cc
  src/elaborator/main/ItemGen_main.cc
  src/elaborator/main/ItemGen_module_inst.cc
  src/elaborator/main/ItemGen_primitive.cc
  src/elaborator/main/ItemGen_taskfunc.cc
  src/elaborator/main/ModuleGen.cc
  src/elaborator/main/StmtGen_assign.cc
  src/elaborator/main/StmtGen_block.cc
  src/elaborator/main/StmtGen_cond.cc
  src/elaborator/main/StmtGen_main.cc
  src/elaborator/main/UdpGen.cc
  )

set (parser_SOURCES
  src/parser/cpt/CptDecl.cc
  src/parser/cpt/CptExpr.cc
  src/parser/cpt/CptFactory.cc
  src/parser/cpt/CptGenerate.cc
  src/parser/cpt/CptIO.cc
  src/parser/cpt/CptInst.cc
  src/parser/cpt/CptItem.cc
  src/parser/cpt/CptMisc.cc
  src/parser/cpt/CptModule.cc
  src/parser/cpt/CptPrimary.cc
  src/parser/cpt/CptSpecItem.cc
  src/parser/cpt/CptStmt.cc
  src/parser/cpt/CptUdp.cc

  src/parser/main/Parser.cc
  src/parser/main/Parser_decl.cc
  src/parser/main/Parser_expr.cc
  src/parser/main/Parser_generate.cc
  src/parser/main/Parser_inst.cc
  src/parser/main/Parser_item.cc
  src/parser/main/Parser_misc.cc
  src/parser/main/Parser_module.cc
  src/parser/main/Parser_port.cc
  src/parser/main/Parser_stmt.cc
  src/parser/main/Parser_udp.cc

  src/parser/pt_mgr/PtDumper.cc
  src/parser/pt_mgr/PtMgr.cc
  src/parser/pt_mgr/PtiBase.cc
  src/parser/pt_mgr/PtiFactory.cc

  src/parser/scanner/InputFile.cc
  src/parser/scanner/InputMgr.cc
  src/parser/scanner/Lex.cc
  src/parser/scanner/LexCondPlugin.cc
  src/parser/scanner/LexMacroPlugin.cc
  src/parser/scanner/LexModulePlugin.cc
  src/parser/scanner/LexModuleState.cc
  src/parser/scanner/LexPlugin.cc
  src/parser/scanner/LexPluginDict.cc
  src/parser/scanner/LexState.cc
  src/parser/scanner/MacroSource.cc
  src/parser/scanner/RawLex.cc
  src/parser/scanner/RsrvWordDic.cc
  src/parser/scanner/StdLexPlugin.cc
  src/parser/scanner/TokenInfo.cc
  src/parser/scanner/TokenList.cc

  src/parser/spt/SptDecl.cc
  src/parser/spt/SptExpr.cc
  src/parser/spt/SptFactory.cc
  src/parser/spt/SptGenerate.cc
  src/parser/spt/SptItem.cc
  src/parser/spt/SptMisc.cc
  src/parser/spt/SptModule.cc
  src/parser/spt/SptSpecItem.cc
  src/parser/spt/SptStmt.cc
  src/parser/spt/SptUdp.cc

  ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc
  )


# Create target for the parser
add_custom_target ( verilog_grammer
  echo "Creating verilog_grammer.cc" )

# Create custom command for bison/yacc (note the DEPENDS)
add_custom_command (
  TARGET verilog_grammer
  COMMAND ${BISON_EXECUTABLE}
  ARGS -y ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/main/verilog_grammer.yy
  -o ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc
  -d -v
  OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc
  ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.hh
  ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.output
  )

# Since verilog_grammer.cc does not exists yet when cmake is run, mark
# it as generated
set_source_files_properties ( ${CMAKE_CURRENT_BINARY_DIR}/verilog_grammer.cc GENERATED )


# ===================================================================
#  ターゲットの設定
# ===================================================================

add_library ( ym_verilog SHARED
  ${common_SOURCES}
  ${elaborator_SOURCES}
  ${parser_SOURCES}
  )

add_dependencies ( ym_verilog verilog_grammer )
