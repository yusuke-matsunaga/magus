#ifndef LALR1SET_H
#define LALR1SET_H

/// @file LALR1Set.h
/// @brief LALR1Set のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"
#include "LR0Set.h"


BEGIN_NAMESPACE_YM

class Token;

//////////////////////////////////////////////////////////////////////
/// @class LALR1Set LALR1Set.h "LALR1Set.h"
//////////////////////////////////////////////////////////////////////
class LALR1Set :
  public LR0Set
{
public:

  /// @brief コンストラクタ
  /// @param[in] grammer 元となる文法
  LALR1Set(Grammer* grammer);

  /// @brief デストラクタ
  ~LALR1Set();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 先読みトークンのリストを返す．
  /// @param[in] state_id 状態番号
  /// @param[in] local_term_id 状態中の項番号
  const vector<const Token*>&
  token_list(ymuint state_id,
	     ymuint local_term_id) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態番号とローカルな項番号から項番号を得る．
  /// @param[in] state_id 状態番号
  /// @param[in] local_term_id 状態中の項番号
  ymuint
  calc_term_id(ymuint state_id,
	       ymuint local_term_id) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 各状態の先頭の項番号を収めた配列
  vector<ymuint> mTermIdTop;

  // 全項数
  ymuint mTermNum;

  // 各項ごとの先読みトークンのリスト
  vector<vector<const Token*> > mTokenList;

};

END_NAMESPACE_YM


#endif // LALR1SET_H
