
/// @file Rule.cc
/// @brief Rule の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Rule.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス Rule
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
Rule::Rule(ymuint id,
	   Token* left,
	   const vector<Token*>& right) :
  mId(id),
  mLeft(left),
  mRight(right)
{
}

// @brief デストラクタ
Rule::~Rule()
{
}

// @brief ID番号を返す．
ymuint
Rule::id() const
{
  return mId;
}

// @brief 左辺のトークンを返す．
const Token*
Rule::left() const
{
  return mLeft;
}

// @brief 右辺の要素数を返す．
ymuint
Rule::right_size() const
{
  return mRight.size();
}

// @brief 右辺のトークンを返す．
// @param[in] pos 位置番号 ( 0 <= pos < right_size() )
const Token*
Rule::right(ymuint pos) const
{
  ASSERT_COND( pos < right_size() );
  return mRight[pos];
}

END_NAMESPACE_YM
