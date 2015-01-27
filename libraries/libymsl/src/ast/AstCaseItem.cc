
/// @file AstCaseItem.cc
/// @brief AstCaseItem の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstCaseItem.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstCaseItem::AstCaseItem(AstExpr* label,
			 AstStatement* stmt,
			 const FileRegion& loc) :
  Ast(loc),
  mLabel(label),
  mStmt(stmt)
{
}

// @brief デストラクタ
AstCaseItem::~AstCaseItem()
{
}

// @brief ラベルを返す．
const AstExpr*
AstCaseItem::label() const
{
  return mLabel;
}

// @brief 本体の文を返す．
const AstStatement*
AstCaseItem::stmt() const
{
  return mStmt;
}

END_NAMESPACE_YM_YMSL
