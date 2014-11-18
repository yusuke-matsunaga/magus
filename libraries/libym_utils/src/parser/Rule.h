#ifndef RULE_H
#define RULE_H

/// @file Rule.h
/// @brief Rule のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Grammer.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class Rule Grammer.h "Grammer.h"
/// @brief 文法規則を表すクラス
//////////////////////////////////////////////////////////////////////
class Rule
{
public:

  /// @brief コンストラクタ
  Rule(ymuint id,
       Token* left,
       const vector<Token*>& right);

  /// @brief デストラクタ
  ~Rule();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 左辺のトークンを返す．
  const Token*
  left() const;

  /// @brief 右辺の要素数を返す．
  ymuint
  right_size() const;

  /// @brief 右辺のトークンを返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < right_size() )
  const Token*
  right(ymuint pos) const;

  /// @brief 最も右にある終端トークンを返す．
  ///
  /// なければ NULL を返す．
  const Token*
  last_terminal() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 左辺のトークン
  Token* mLeft;

  // 右辺のトークンリスト
  vector<Token*> mRight;

};

END_NAMESPACE_YM

#endif // RULE_H
