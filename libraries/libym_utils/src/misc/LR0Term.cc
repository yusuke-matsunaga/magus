
/// @file LR0Term.cc
/// @brief LR0Term の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LR0Term.h"
#include "Rule.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LR0Term
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] rule 文法規則
// @param[in] pos ハンドルの位置
LR0Term::LR0Term(const Rule* rule,
		 ymuint pos) :
  mRule(rule),
  mPos(pos)
{
}

// @brief デストラクタ
LR0Term::~LR0Term()
{
}

// @brief 文法規則を返す．
const Rule*
LR0Term::rule() const
{
  return mRule;
}

// @brief dot の位置を返す．
ymuint
LR0Term::dot_pos() const
{
  return mPos;
}

// @brief dot の直後のトークンを返す．
const Token*
LR0Term::next_token() const
{
  if ( mPos < mRule->right_size() ) {
    return mRule->right(mPos);
  }
  return NULL;
}

END_NAMESPACE_YM
