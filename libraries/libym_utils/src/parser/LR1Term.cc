
/// @file LR1Term.cc
/// @brief LR1Term の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LR1Term.h"
#include "Rule.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス LR1Term
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] rule 文法規則
// @param[in] pos ハンドルの位置
// @param[in] token 先読みトークン
LR1Term::LR1Term(const Rule* rule,
		 ymuint pos,
		 const Token* token) :
  mRule(rule),
  mPos(pos),
  mToken(token)
{
}

// @brief デストラクタ
LR1Term::~LR1Term()
{
}

// @brief 文法規則を返す．
const Rule*
LR1Term::rule() const
{
  return mRule;
}

// @brief dot の位置を返す．
ymuint
LR1Term::dot_pos() const
{
  return mPos;
}

// @brief dot の直後のトークンを返す．
const Token*
LR1Term::next_token() const
{
  if ( mPos < mRule->right_size() ) {
    return mRule->right(mPos);
  }
  return NULL;
}

// @brief 先読みトークンを返す．
const Token*
LR1Term::token() const
{
  return mToken;
}

END_NAMESPACE_YM
