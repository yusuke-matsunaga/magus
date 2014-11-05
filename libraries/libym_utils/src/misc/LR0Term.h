#ifndef LR0TERM_H
#define LR0TERM_H

/// @file LR0Term.h
/// @brief LR0Term のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class Token;
class Rule;

//////////////////////////////////////////////////////////////////////
/// @class LR0Term LR0Term.h "LR0Term.h"
/// @brief LR(0)項を表すクラス
//////////////////////////////////////////////////////////////////////
class LR0Term
{
public:

  /// @brief コンストラクタ
  /// @param[in] rule 文法規則
  /// @param[in] pos ハンドルの位置
  LR0Term(const Rule* rule = NULL,
	  ymuint pos = 0);

  /// @brief デストラクタ
  ~LR0Term();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文法規則
  const Rule* mRule;

  // dot の位置
  ymuint mPos;

};

/// @brief 等価比較演算子
inline
bool
operator==(const LR0Term& left,
	   const LR0Term& right)
{
  return left.rule() == right.rule() && left.dot_pos() == right.dot_pos();
}

END_NAMESPACE_YM

#endif // LR0TERM_H
