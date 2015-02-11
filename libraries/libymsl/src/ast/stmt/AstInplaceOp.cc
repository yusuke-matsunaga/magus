
/// @file AstInplaceOp.cc
/// @brief AstInplaceOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstInplaceOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstInplaceOp
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] opcode オペコード
// @param[in] left 左辺
// @param[in] right 右辺
// @param[in] loc ファイル位置
AstInplaceOp::AstInplaceOp(OpCode opcode,
			   AstExpr* left,
			   AstExpr* right,
			   const FileRegion& loc) :
  AstStatement(loc),
  mOpCode(opcode),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
AstInplaceOp::~AstInplaceOp()
{
}

// @brief 種類を返す．
AstStatement::Type
AstInplaceOp::stmt_type() const
{
  return kInplaceOp;
}

// @brief オペコードを返す．
//
// kInplaceOp のみ有効
OpCode
AstInplaceOp::opcode() const
{
  return mOpCode;
}

// @brief 左辺式を返す．
//
// kInplaceOp のみ有効
const AstExpr*
AstInplaceOp::lhs_expr() const
{
  return mLeft;
}

// @brief 式を返す．
//
// kInplaceOp,
// kDoWhile, kFor, kIf, kWhile, kSwitch
// kExprStmt, kReturn, kVarDecl のみ有効
const AstExpr*
AstInplaceOp::expr() const
{
  return mRight;
}

END_NAMESPACE_YM_YMSL
