#ifndef TOKEN_H
#define TOKEN_H

/// @file Token.h
/// @brief Token のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Grammer.h"


BEGIN_NAMESPACE_YM

class Rule;

//////////////////////////////////////////////////////////////////////
/// @class Token Token.h "Token.h"
/// @brief トークンを表すクラス
//////////////////////////////////////////////////////////////////////
class Token
{
  friend class Grammer;

public:

  /// @brief コンストラクタ
  /// @param[in] id トークンID
  /// @param[in] str 文字列
  /// @param[in] pri 優先順位
  /// @param[in] assoc 結合規則
  Token(ymuint id,
	string str,
	ymuint pri = 0,
	AssocType assoc = kNotDefined);

  /// @brief デストラクタ
  ~Token();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ID番号を返す．
  ymuint
  id() const;

  /// @brief 文字列を返す．
  string
  str() const;

  /// @brief 優先順位を返す．
  ymuint
  priority() const;

  /// @brief 結合性を返す．
  AssocType
  assoc_type() const;

  /// @brief 文法規則のリストを返す．
  const vector<const Rule*>&
  rule_list() const;

  /// @brief FIRST を返す．
  const vector<const Token*>&
  first() const;

  /// @brief FOLLOW を返す．
  const vector<const Token*>&
  follow() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ID番号
  ymuint mId;

  // 文字列
  string mStr;

  // 優先順位
  // 0 で優先順位なし
  ymuint mPri;

  // 結合性
  AssocType mAssocType;

  // このトークンを左辺に持つ文法規則のリスト
  vector<const Rule*> mRuleList;

  // FIRST リスト
  vector<const Token*> mFirst;

  // FOLLOW リスト
  vector<const Token*> mFollow;

};

END_NAMESPACE_YM

#endif // TOKEN_H
