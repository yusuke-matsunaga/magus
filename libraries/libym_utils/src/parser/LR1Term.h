#ifndef LR1TERM_H
#define LR1TERM_H

/// @file LR1Term.h
/// @brief LR1Term のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class Token;
class Rule;

//////////////////////////////////////////////////////////////////////
/// @class LR1Term LR1Term.h "LR1Term.h"
/// @brief LR(1)項を表すクラス
//////////////////////////////////////////////////////////////////////
class LR1Term
{
public:

  /// @brief コンストラクタ
  /// @param[in] rule 文法規則
  /// @param[in] pos ハンドルの位置
  /// @param[in] token 先読みトークン
  LR1Term(const Rule* rule = NULL,
	  ymuint pos = 0,
	  const Token* token = NULL);

  /// @brief デストラクタ
  ~LR1Term();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 文法規則を返す．
  const Rule*
  rule() const;

  /// @brief dot の位置を返す．
  ///
  /// 値の範囲は 0 〜 rule()->right_size()
  ymuint
  dot_pos() const;

  /// @brief dot の直後のトークンを返す．
  ///
  /// dot が末尾にある場合には NULL を返す．
  const Token*
  next_token() const;

  /// @brief 先読みトークンを返す．
  const Token*
  token() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文法規則
  const Rule* mRule;

  // dot の位置
  ymuint mPos;

  // 先読みトークン
  const Token* mToken;

};

/// @brief 等価比較演算子
inline
bool
operator==(const LR1Term& left,
	   const LR1Term& right)
{
  return left.rule() == right.rule() && left.dot_pos() == right.dot_pos() && left.token() == right.token();
}

END_NAMESPACE_YM

#endif // LR1TERM_H
