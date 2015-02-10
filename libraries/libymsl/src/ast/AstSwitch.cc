
/// @file AstSwitch.cc
/// @brief AstSwitch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstSwitch.h"
#include "AstCaseItem.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstSwitch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 条件式
// @param[in] case_num case 文の数
// @param[in] case_list case リスト
// @param[in] loc ファイル位置
AstSwitch::AstSwitch(AstExpr* expr,
		     ymuint case_num,
		     AstCaseItem** case_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr),
  mNum(case_num),
  mCaseItemList(case_list)
{
}

// @brief デストラクタ
AstSwitch::~AstSwitch()
{
}

// @brief 種類を返す．
AstStatement::Type
AstSwitch::stmt_type() const
{
  return kSwitch;
}

// @brief switch 文の case 数を返す．
//
// kSwitch のみ有効
ymuint
AstSwitch::switch_num() const
{
  return mNum;
}

// @brief switch 文の case ラベルを返す．
// @param[in] pos 位置 ( 0 <= pos < switch_num() )
//
// kSwitch のみ有効
const AstExpr*
AstSwitch::case_label(ymuint pos) const
{
  ASSERT_COND( pos < switch_num() );
  return mCaseItemList[pos]->label();
}

// @brief switch 文の case ブロックを返す．
// @param[in] pos 位置 ( 0 <= pos < switch_num() )
//
// kSwitch のみ有効
const AstStatement*
AstSwitch::case_stmt(ymuint pos) const
{
  ASSERT_COND( pos < switch_num() );
  return mCaseItemList[pos]->stmt();
}

END_NAMESPACE_YM_YMSL
