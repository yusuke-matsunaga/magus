
/// @file AstToplevel.cc
/// @brief AstToplevel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstToplevel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstToplevel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] head_num ヘッダのリストの要素数
// @param[in] head_list ヘッダのリスト
// @param[in] stmt_num 文のリストの要素数
// @param[in] stmt_list 文のリスト
// @param[in] loc ファイル位置
AstToplevel::AstToplevel(ymuint head_num,
			 AstStatement** head_list,
			 ymuint stmt_num,
			 AstStatement** stmt_list,
			 const FileRegion& loc) :
  AstBlockStmt(stmt_num, stmt_list, loc),
  mHeadNum(head_num),
  mHeadList(head_list)
{
}

// @brief デストラクタ
AstToplevel::~AstToplevel()
{
}

// @brief 種類を返す．
AstStatement::Type
AstToplevel::stmt_type() const
{
  return kToplevel;
}

// @brief ヘッダのリストの要素数を返す．
//
// AstToplevel のみ有効
ymuint
AstToplevel::headlist_num() const
{
  return mHeadNum;
}

// @brief ヘッダを返す．
// @param[in] pos 位置 ( 0 <= pos < headlist_num() )
//
// AstToplevel のみ有効
const AstStatement*
AstToplevel::headlist_elem(ymuint pos) const
{
  ASSERT_COND( pos < headlist_num() );
  return mHeadList[pos];
}

END_NAMESPACE_YM_YMSL
