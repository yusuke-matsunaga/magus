
/// @file IrPrinter.cc
/// @brief IrPrinter の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrPrinter.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrPrinter
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] s 出力先のストリーム
IrPrinter::IrPrinter(ostream& s) :
  mS(s)
{
}

// @brief デストラクタ
IrPrinter::~IrPrinter()
{
}

// @brief ノードの内容を文として出力する．
void
IrPrinter::print_statement(IrNode* node)
{
  switch ( node->opcode() ) {
  case kOpAssign:
    break;

  case kOpFuncCall:
    break;

  case kOpJump:
    break;

  case kOpBranchTrue:
    break;

  case kOpBranchFalse:
    break;

  case kOpLabel:
    break;

  case kOpHalt:
    break;

  default:
    // 式のはず．
    print_expr(node);
    break;
  }
}

// @brief ノードの内容を式として出力する．
void
IrPrinter::print_expr(IrNode* node)
{
}

END_NAMESPACE_YM_YMSL
