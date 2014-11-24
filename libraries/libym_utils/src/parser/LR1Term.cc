
/// @file LR1Term.cc
/// @brief LR1Term の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "LR1Term.h"
#include "Rule.h"
#include "Token.h"


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

// @brief ストリーム出力
ostream&
operator<<(ostream& s,
	   const LR1Term& term)
{
  const Rule* rule = term.rule();
  ymuint pos = term.dot_pos();
  const Token* left = rule->left();
  s << "  " << left->str() << " ->";
  ymuint nr = rule->right_size();
  for (ymuint j = 0; j < nr; ++ j) {
    if ( j == pos ) {
      s << " .";
    }
    s << " " << rule->right(j)->str();
  }
  if ( pos == nr ) {
    s << " .";
  }
  const Token* la = term.token();
  if ( la != NULL ) {
    s << ", " << la->str();
  }
  return s;
}

END_NAMESPACE_YM
