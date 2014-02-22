#ifndef VERILOG_PT_PTP_H
#define VERILOG_PT_PTP_H

/// @file verilog/pt/PtP.h
/// @brief Pt クラスの名前の宣言のみおこなうヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 列挙型の定義
//////////////////////////////////////////////////////////////////////

/// @brief IO の種類を表す列挙型
enum tPtIOType {
  /// @brief 入力
  kPtIO_Input  = 0,
  /// @brief 出力
  kPtIO_Output = 1,
  /// @brief 入出力
  kPtIO_Inout  = 2
};

/// @brief PtDeclH の派生クラスを区別するための列挙型
enum tPtDeclType {
  kPtDecl_Param,      //< @brief parameter
  kPtDecl_LocalParam, //< @brief localparam
  kPtDecl_Reg,        //< @brief reg
  kPtDecl_Var,        //< @brief variables
  kPtDecl_Genvar,     //< @brief genvar
  kPtDecl_Net,        //< @brief net
  kPtDecl_Event,      //< @brief named event
  kPtDecl_SpecParam   //< @brief specparam
};

/// @brief PtItem の派生クラスの識別コード
enum tPtItemType {
  kPtItem_DefParam,
  kPtItem_ContAssign,
  kPtItem_Initial,
  kPtItem_Always,
  kPtItem_Task,
  kPtItem_Func,
  kPtItem_GateInst,
  kPtItem_MuInst,
  kPtItem_SpecItem,
  kPtItem_SpecPath,
  kPtItem_Generate,
  kPtItem_GenBlock,
  kPtItem_GenIf,
  kPtItem_GenCase,
  kPtItem_GenFor
};

/// @brief PtStmt の派生クラスの型を表す列挙型
enum tPtStmtType {
  kPtDisableStmt,
  kPtEnableStmt,
  kPtSysEnableStmt,
  kPtDcStmt,
  kPtEcStmt,
  kPtAssignStmt,
  kPtNbAssignStmt,
  kPtEventStmt,
  kPtNullStmt,
  kPtIfStmt,
  kPtCaseStmt,
  kPtCaseXStmt,
  kPtCaseZStmt,
  kPtWaitStmt,
  kPtForeverStmt,
  kPtRepeatStmt,
  kPtWhileStmt,
  kPtForStmt,
  kPtPcAssignStmt,
  kPtDeassignStmt,
  kPtForceStmt,
  kPtReleaseStmt,
  kPtParBlockStmt,
  kPtSeqBlockStmt,
  kPtNamedParBlockStmt,
  kPtNamedSeqBlockStmt
};

/// @brief PtExpr の派生クラスを識別するための列挙型
enum tPtExprType {
  /// @brief 演算子
  kPtOprExpr,
  /// @brief 関数呼び出し
  kPtFuncCallExpr,
  /// @brief システム関数呼び出し
  kPtSysFuncCallExpr,
  /// @brief プライマリ
  kPtPrimaryExpr,
  /// @brief 定数
  kPtConstExpr
};

/// @brief コントロールの型を区別するための列挙型
enum tPtCtrlType {
  /// @brief delay control
  kPtDelayControl,
  /// @brief event control
  kPtEventControl,
  /// @brief repeat control
  kPtRepeatControl
};


//////////////////////////////////////////////////////////////////////
// クラス名の前方宣言
//////////////////////////////////////////////////////////////////////

// in PtBase.h
class PtBase;
class PtNamedBase;

// in PtUdp.h
class PtUdp;
class PtUdpEntry;
class PtUdpValue;

// in PtModule.h
class PtModule;

// in PtPort.h
class PtPort;

// in PtDecl.h
class PtIOHead;
class PtIOItem;
class PtDeclHead;
class PtDeclItem;
class PtRange;

// in PtItem.h
class PtItem;
class PtDefParam;
class PtContAssign;
class PtInst;
class PtGenCaseItem;
class PtPathDecl;
class PtPathDelay;

// in PtStmt.h
class PtStmt;
class PtCaseItem;

// in PtExpr.h
class PtExpr;

// in PtMisc.h
class PtStrength;
class PtDelay;
class PtControl;
class PtConnection;
class PtNameBranch;
class PtAttrInst;
class PtAttrSpec;

// in PtArray.h
template <typename T>
class PtArray;

typedef PtArray<const PtPort> PtPortArray;
typedef PtArray<const PtUdpEntry> PtUdpEntryArray;
typedef PtArray<const PtUdpValue> PtUdpValueArray;
typedef PtArray<const PtIOHead> PtIOHeadArray;
typedef PtArray<const PtIOItem> PtIOItemArray;
typedef PtArray<const PtDeclHead> PtDeclHeadArray;
typedef PtArray<const PtDeclItem> PtDeclItemArray;
typedef PtArray<const PtItem> PtItemArray;
typedef PtArray<const PtInst> PtInstArray;
typedef PtArray<const PtDefParam> PtDefParamArray;
typedef PtArray<const PtContAssign> PtContAssignArray;
typedef PtArray<const PtGenCaseItem> PtGenCaseItemArray;
typedef PtArray<const PtStmt> PtStmtArray;
typedef PtArray<const PtCaseItem> PtCaseItemArray;
typedef PtArray<const PtExpr> PtExprArray;
typedef PtArray<const PtConnection> PtConnectionArray;
typedef PtArray<const PtRange> PtRangeArray;
typedef PtArray<const PtNameBranch> PtNameBranchArray;
typedef PtArray<const PtAttrInst> PtAttrInstArray;
typedef PtArray<const PtAttrSpec> PtAttrSpecArray;

END_NAMESPACE_YM_VERILOG

BEGIN_NAMESPACE_HASH

// PtModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<nsYm::nsVerilog::PtModule*>
{
  ymuint
  operator()(nsYm::nsVerilog::PtModule* module) const
  {
    return reinterpret_cast<ympuint>(module)/sizeof(void*);
  }
};

// const PtModule へのポインタをキーにしたハッシュ関数クラスの定義
template <>
struct hash<const nsYm::nsVerilog::PtModule*>
{
  ymuint
  operator()(const nsYm::nsVerilog::PtModule* module) const
  {
    return reinterpret_cast<ympuint>(module)/sizeof(void*);
  }
};

END_NAMESPACE_HASH

#endif // VERILOG_PT_PTP_H
