
/// @file Token.cc
/// @brief Token の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Token.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス Token
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] id トークンID
// @param[in] str 文字列
// @param[in] pri 優先順位
// @param[in] assoc 結合規則
Token::Token(ymuint id,
	     string str,
	     ymuint pri,
	     AssocType assoc) :
  mId(id),
  mStr(str),
  mPri(pri),
  mAssocType(assoc)
{
}

// @brief デストラクタ
Token::~Token()
{
}

// @brief ID番号を返す．
ymuint
Token::id() const
{
  return mId;
}

// @brief 文字列を返す．
string
Token::str() const
{
  return mStr;
}

// @brief 優先順位を返す．
ymuint
Token::priority() const
{
  return mPri;
}

// @brief 結合性を返す．
AssocType
Token::assoc_type() const
{
  return mAssocType;
}

// @brief 文法規則のリストを返す．
const vector<const Rule*>&
Token::rule_list() const
{
  return mRuleList;
}

// @brief FIRST を返す．
const vector<const Token*>&
Token::first() const
{
  return mFirst;
}

// @brief FOLLOW を返す．
const vector<const Token*>&
Token::follow() const
{
  return mFollow;
}

END_NAMESPACE_YM
