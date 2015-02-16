
/// @file AstBlockStmt.cc
/// @brief AstBlockStmt の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBlockStmt
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] stmt_num 文のリストの要素数
// @param[in] stmt_list 文のリスト
// @param[in] loc ファイル位置
AstBlockStmt::AstBlockStmt(ymuint stmt_num,
			   AstStatement** stmt_list,
			   const FileRegion& loc) :
  AstStatement(loc),
  mStmtNum(stmt_num),
  mStmtList(stmt_list)
{
}

// @brief デストラクタ
AstBlockStmt::~AstBlockStmt()
{
}

// @brief 種類を返す．
AstStatement::Type
AstBlockStmt::stmt_type() const
{
  return kBlock;
}

// @brief 文のリストの要素数を返す．
//
// AstBlockStmt のみ有効
ymuint
AstBlockStmt::stmtlist_num() const
{
  return mStmtNum;
}

// @brief 文のリストの要素を返す．
// @param[in] pos 位置 ( 0 <= pos < stmt_num() )
//
// AstBlockStmt のみ有効
const AstStatement*
AstBlockStmt::stmtlist_elem(ymuint pos) const
{
  ASSERT_COND( pos < stmtlist_num() );
  return mStmtList[pos];
}

END_NAMESPACE_YM_YMSL
