
# ===================================================================
# インクルードパスの設定
# ===================================================================
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_BINARY_DIR}
  include
  ${GMP_INCLUDE_DIR}
  )


# ===================================================================
#  ターゲットの設定
# ===================================================================
add_library(ym_logic SHARED
  src/aig/AigMgr.cc
  src/aig/AigMgrImpl.cc
  src/aig/AigMgrImpl.h
  src/aig/AigNode.cc
  src/aig/AigNode.h
  src/bdd/base/AeOp.cc
  src/bdd/base/AeOp.h
  src/bdd/base/AndOp.cc
  src/bdd/base/AndOp.h
  src/bdd/base/Bdd.cc
  src/bdd/base/BddBinOp.cc
  src/bdd/base/BddBinOp.h
  src/bdd/base/BddList.cc
  src/bdd/base/BddLitSet.cc
  src/bdd/base/BddMgr.cc
  src/bdd/base/BddMgrImpl.cc
  src/bdd/base/BddTriOp.cc
  src/bdd/base/BddTriOp.h
  src/bdd/base/BddUniOp.cc
  src/bdd/base/BddUniOp.h
  src/bdd/base/BddVarSet.cc
  src/bdd/base/BddVector.cc
  src/bdd/base/CofOp.cc
  src/bdd/base/CofOp.h
  src/bdd/base/CompTbl.cc
  src/bdd/base/CompTbl.h
  src/bdd/base/CompTbl1.cc
  src/bdd/base/CompTbl1.h
  src/bdd/base/CompTbl2.cc
  src/bdd/base/CompTbl2.h
  src/bdd/base/CompTbl3.cc
  src/bdd/base/CompTbl3.h
  src/bdd/base/CountOp.cc
  src/bdd/base/CountOp.h
  src/bdd/base/Dumper.cc
  src/bdd/base/Dumper.h
  src/bdd/base/GcOp.cc
  src/bdd/base/GcOp.h
  src/bdd/base/IdMgr.cc
  src/bdd/base/IdMgr.h
  src/bdd/base/IntsecOp.cc
  src/bdd/base/IntsecOp.h
  src/bdd/base/IteOp.cc
  src/bdd/base/IteOp.h
  src/bdd/base/McOp.cc
  src/bdd/base/McOp.h
  src/bdd/base/Printer.cc
  src/bdd/base/Printer.h
  src/bdd/base/Restorer.cc
  src/bdd/base/Restorer.h
  src/bdd/base/SmoothOp.cc
  src/bdd/base/SmoothOp.h
  src/bdd/base/SupOp.cc
  src/bdd/base/SupOp.h
  src/bdd/base/SymOp.cc
  src/bdd/base/SymOp.h
  src/bdd/base/W0Op.cc
  src/bdd/base/W0Op.h
  src/bdd/base/W1Op.cc
  src/bdd/base/W1Op.h
  src/bdd/base/XcOp.cc
  src/bdd/base/XcOp.h
  src/bdd/base/XorOp.cc
  src/bdd/base/XorOp.h
  src/bdd/bmc/BddMgrClassic.cc
  src/bdd/bmc/BddMgrClassic.h
  src/bdd/bmc/BmcCompTbl.cc
  src/bdd/bmc/BmcCompTbl.h
  src/bdd/bmc/BmcNode.h
  src/bdd/bmc/BmcVar.h
  src/bdd/bmc/bmc_isop.cc
  src/bdd/bmc/bmc_ite.cc
  src/bdd/bmc/bmc_ls.cc
  src/bdd/bmc/bmc_onepath.cc
  src/bdd/bmc/bmc_vs.cc
  src/bdd/bmm/BddMgrModern.cc
  src/bdd/bmm/BddMgrModern.h
  src/bdd/bmm/BmmCompTbl.cc
  src/bdd/bmm/BmmCompTbl.h
  src/bdd/bmm/BmmNode.h
  src/bdd/bmm/BmmVar.h
  src/bdd/bmm/bmm_isop.cc
  src/bdd/bmm/bmm_ite.cc
  src/bdd/bmm/bmm_ls.cc
  src/bdd/bmm/bmm_onepath.cc
  src/bdd/bmm/bmm_vs.cc
#  src/bdd/dec/Bidec.cc
#  src/bdd/dec/Dg.cc
#  src/bdd/dec/DgMgr.cc
#  src/bdd/dec/DgNode.cc
#  src/bdd/dec/DgNode.h
#  src/bdd/dec/PrimDec.cc
  src/cnfdd/BinOp.cc
  src/cnfdd/BinOp.h
  src/cnfdd/CNFdd.cc
  src/cnfdd/CNFddEdge.h
  src/cnfdd/CNFddMgr.cc
  src/cnfdd/CNFddMgrImpl.cc
  src/cnfdd/CNFddMgrImpl.h
  src/cnfdd/CNFddNode.h
  src/cnfdd/CNFddVar.h
  src/cnfdd/Cof0Op.cc
  src/cnfdd/Cof0Op.h
  src/cnfdd/CofNOp.cc
  src/cnfdd/CofNOp.h
  src/cnfdd/CofPOp.cc
  src/cnfdd/CofPOp.h
  src/cnfdd/CompTbl.cc
  src/cnfdd/CompTbl.h
  src/cnfdd/ConOp.cc
  src/cnfdd/ConOp.h
  src/cnfdd/CutOp.cc
  src/cnfdd/CutOp.h
  src/cnfdd/DiffOp.cc
  src/cnfdd/DiffOp.h
  src/cnfdd/DiffOp2.cc
  src/cnfdd/DiffOp2.h
  src/cnfdd/DisOp.cc
  src/cnfdd/DisOp.h
  src/cnfdd/Dumper.cc
  src/cnfdd/Dumper.h
  src/cnfdd/IdMgr.cc
  src/cnfdd/IdMgr.h
  src/cnfdd/LitNOp.cc
  src/cnfdd/LitNOp.h
  src/cnfdd/LitPOp.cc
  src/cnfdd/LitPOp.h
  src/cnfdd/MergeOp.cc
  src/cnfdd/MergeOp.h
  src/cnfdd/MinOp.cc
  src/cnfdd/MinOp.h
  src/cnfdd/Op.h
  src/cnfdd/Printer.cc
  src/cnfdd/Printer.h
  src/cnfdd/Printer2.cc
  src/cnfdd/Printer2.h
  src/cnfdd/Restorer.cc
  src/cnfdd/Restorer.h
  src/cnfdd/SupOp.cc
  src/cnfdd/SupOp.h
  src/cnfdd/UniNOp.cc
  src/cnfdd/UniNOp.h
  src/cnfdd/UniOp.cc
  src/cnfdd/UniOp.h
  src/cnfdd/UniVOp.cc
  src/cnfdd/UniVOp.h
  src/cnfdd/count.cc
  src/expr/Expr.cc
  src/expr/ExprMgr.cc
  src/expr/ExprMgr.h
  src/expr/ExprNode.cc
  src/expr/ExprNode.h
  src/expr/ExprNodePtr.h
  src/expr/ExprParser.cc
  src/expr/ExprParser.h
  src/expr/ExprWriter.cc
  src/expr/Literal.cc
  src/expr/SopLit.h
  src/expr/expr_types.h
  src/npn/NpnBaseConf.cc
  src/npn/NpnBaseConf.h
  src/npn/NpnConf.cc
  src/npn/NpnConf.h
  src/npn/NpnMap.cc
  src/npn/NpnMapM.cc
  src/npn/NpnMgr.cc
  src/npn/NpnMgrImpl.cc
  src/npn/NpnMgrImpl.h
  src/npn/NpnRawSig.cc
  src/npn/NpnRawSig.h
  src/sat/SatMsgHandlerImpl1.cc
  src/sat/SatSolver.cc
  src/sat/SatSolverImpl.h
  src/sat/MiniSat/Global.h
  src/sat/MiniSat/Heap.h
  src/sat/MiniSat/Main.C
  src/sat/MiniSat/SatSolverMiniSat.cc
  src/sat/MiniSat/SatSolverMiniSat.h
  src/sat/MiniSat/Solver.C
  src/sat/MiniSat/Solver.h
  src/sat/MiniSat/SolverTypes.h
  src/sat/MiniSat/Sort.h
  src/sat/MiniSat/VarOrder.h
  src/sat/MiniSat2/Alg.h
  src/sat/MiniSat2/Alloc.h
  src/sat/MiniSat2/Heap.h
  src/sat/MiniSat2/IntTypes.h
  src/sat/MiniSat2/Map.h
  src/sat/MiniSat2/Options.cc
  src/sat/MiniSat2/Options.h
  src/sat/MiniSat2/ParseUtils.h
  src/sat/MiniSat2/Queue.h
  src/sat/MiniSat2/SatSolverMiniSat2.cc
  src/sat/MiniSat2/SatSolverMiniSat2.h
  src/sat/MiniSat2/Solver.cc
  src/sat/MiniSat2/Solver.h
  src/sat/MiniSat2/SolverTypes.h
  src/sat/MiniSat2/Sort.h
  src/sat/MiniSat2/System.cc
  src/sat/MiniSat2/System.h
  src/sat/MiniSat2/Vec.h
  src/sat/MiniSat2/XAlloc.h
  src/sat/dimacs/DimacsParser.cc
  src/sat/dimacs/DimacsParserImpl.h
  src/sat/dimacs/DimacsScanner.cc
  src/sat/dimacs/DimacsScanner.h
  src/sat/dimacs/DimacsVerifier.cc
  src/sat/dimacs/SatDimacsHandler.cc
  src/sat/ymsat/AssignList.cc
  src/sat/ymsat/AssignList.h
  src/sat/ymsat/SaBase.cc
  src/sat/ymsat/SaBase.h
  src/sat/ymsat/SaFactory.cc
  src/sat/ymsat/SaUIP1.cc
  src/sat/ymsat/SaUIP1.h
  src/sat/ymsat/SaUIP2.cc
  src/sat/ymsat/SaUIP2.h
  src/sat/ymsat/SatAnalyzer.h
  src/sat/ymsat/SatClause.cc
  src/sat/ymsat/SatClause.h
  src/sat/ymsat/SatReason.h
  src/sat/ymsat/VarHeap.cc
  src/sat/ymsat/VarHeap.h
  src/sat/ymsat/Watcher.h
  src/sat/ymsat/YmSat.cc
  src/sat/ymsat/YmSat.h
  src/sat/ymsat/YmSatR.cc
  src/sat/ymsat/YmSatR.h
  src/smt/core/SmtFunImpl.cc
  src/smt/core/SmtFunImpl.h
  src/smt/core/SmtFunValidatorImpl.cc
  src/smt/core/SmtFunValidatorImpl.h
  src/smt/core/SmtSolverImpl.cc
  src/smt/core/SmtSolverImpl.h
  src/smt/core/SmtSortImpl.cc
  src/smt/core/SmtSortImpl.h
  src/smt/core/SmtTermImpl.cc
  src/smt/core/SmtTermImpl.h
  src/smt/core/SmtVarImpl.cc
  src/smt/core/SmtVarImpl.h
#  src/smt/smtlibv2/main/NameObjImpl.cc
#  src/smt/smtlibv2/main/NameObjImpl.h
#  src/smt/smtlibv2/main/SmtCompTerm.cc
#  src/smt/smtlibv2/main/SmtCompTerm.h
#  src/smt/smtlibv2/main/SmtConstTerm.cc
#  src/smt/smtlibv2/main/SmtConstTerm.h
#  src/smt/smtlibv2/main/SmtFunImpl.cc
#  src/smt/smtlibv2/main/SmtFunImpl.h
#  src/smt/smtlibv2/main/SmtFunMgr.cc
#  src/smt/smtlibv2/main/SmtFunMgr.h
#  src/smt/smtlibv2/main/SmtFunTerm.cc
#  src/smt/smtlibv2/main/SmtFunTerm.h
#  src/smt/smtlibv2/main/SmtIdImpl.cc
#  src/smt/smtlibv2/main/SmtIdImpl.h
#  src/smt/smtlibv2/main/SmtIdMgr.cc
#  src/smt/smtlibv2/main/SmtIdMgr.h
#  src/smt/smtlibv2/main/SmtIdTerm.cc
#  src/smt/smtlibv2/main/SmtIdTerm.h
#  src/smt/smtlibv2/main/SmtListTerm.cc
#  src/smt/smtlibv2/main/SmtListTerm.h
#  src/smt/smtlibv2/main/SmtMgr.cc
#  src/smt/smtlibv2/main/SmtNameMgr.cc
#  src/smt/smtlibv2/main/SmtNameMgr.h
#  src/smt/smtlibv2/main/SmtNameObj.h
#  src/smt/smtlibv2/main/SmtSortImpl.cc
#  src/smt/smtlibv2/main/SmtSortImpl.h
#  src/smt/smtlibv2/main/SmtSortMgr.cc
#  src/smt/smtlibv2/main/SmtSortMgr.h
#  src/smt/smtlibv2/main/SmtTermImpl.cc
#  src/smt/smtlibv2/main/SmtTermImpl.h
#  src/smt/smtlibv2/main/SmtTermMgr.cc
#  src/smt/smtlibv2/main/SmtTermMgr.h
#  src/smt/smtlibv2/main/SmtVarFunImpl.cc
#  src/smt/smtlibv2/main/SmtVarFunImpl.h
#  src/smt/smtlibv2/main/SmtVarTerm.cc
#  src/smt/smtlibv2/main/SmtVarTerm.h
#  src/smt/smtlibv2/main/StackPage.h
#  src/smt/smtlibv2/main/print.cc
  src/smt/smtlibv2/parser/SmtLibNodeImpl.cc
  src/smt/smtlibv2/parser/SmtLibNodeImpl.h
  src/smt/smtlibv2/parser/SmtLibParser.cc
  src/smt/smtlibv2/parser/SmtLibScanner.cc
  src/smt/smtlibv2/parser/SmtLibScanner.h
  src/smt/smtlibv2/shell/IdImpl.cc
  src/smt/smtlibv2/shell/IdImpl.h
  src/smt/smtlibv2/shell/IdMgr.cc
  src/smt/smtlibv2/shell/IdMgr.h
  src/smt/smtlibv2/shell/NameMgr.cc
  src/smt/smtlibv2/shell/NameMgr.h
  src/smt/smtlibv2/shell/NameObj.h
  src/smt/smtlibv2/shell/NameObjImpl.cc
  src/smt/smtlibv2/shell/NameObjImpl.h
  src/smt/smtlibv2/shell/ShellImpl.cc
  src/smt/smtlibv2/shell/ShellImpl.h
  src/smt/smtlibv2/shell/SmtLibShell.cc
  src/smt/smtlibv2/shell/SortElem.cc
  src/smt/smtlibv2/shell/SortElem.h
  src/smt/smtlibv2/shell/SortMgr.cc
  src/smt/smtlibv2/shell/SortMgr.h
  src/tvfunc/TvFunc.cc
  src/tvfunc/TvFuncM.cc
  src/zdd/CapOp.cc
  src/zdd/CapOp.h
  src/zdd/Cof0Op.cc
  src/zdd/Cof0Op.h
  src/zdd/Cof1Op.cc
  src/zdd/Cof1Op.h
  src/zdd/CompTbl.cc
  src/zdd/CompTbl.h
  src/zdd/CupOp.cc
  src/zdd/CupOp.h
  src/zdd/DiffOp.cc
  src/zdd/DiffOp.h
  src/zdd/Dumper.cc
  src/zdd/Dumper.h
  src/zdd/IdMgr.cc
  src/zdd/IdMgr.h
  src/zdd/Makefile.am
  src/zdd/MergeOp.cc
  src/zdd/MergeOp.h
  src/zdd/MergeOp2.cc
  src/zdd/MergeOp2.h
  src/zdd/Ms2Op.cc
  src/zdd/Ms2Op.h
  src/zdd/MsOp.cc
  src/zdd/MsOp.h
  src/zdd/NeOp.cc
  src/zdd/NeOp.h
  src/zdd/Printer.cc
  src/zdd/Printer.h
  src/zdd/Printer2.cc
  src/zdd/Printer2.h
  src/zdd/Restorer.cc
  src/zdd/Restorer.h
  src/zdd/SupOp.cc
  src/zdd/SupOp.h
  src/zdd/SwapOp.cc
  src/zdd/SwapOp.h
  src/zdd/Zdd.cc
  src/zdd/ZddBinOp.cc
  src/zdd/ZddBinOp.h
  src/zdd/ZddEdge.h
  src/zdd/ZddList.cc
  src/zdd/ZddMgr.cc
  src/zdd/ZddMgrImpl.cc
  src/zdd/ZddMgrImpl.h
  src/zdd/ZddNode.h
  src/zdd/ZddOp.h
  src/zdd/ZddUniOp.cc
  src/zdd/ZddUniOp.h
  src/zdd/ZddVar.h
  src/zdd/ZddVector.cc
  src/zdd/count.cc
  )

target_link_libraries(ym_logic
  ${GMP_LIBRARY}
  )