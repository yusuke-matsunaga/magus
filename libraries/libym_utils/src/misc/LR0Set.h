#ifndef LR0SET_H
#define LR0SET_H

/// @file LR0Set.h
/// @brief LR0Set のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmTools.h"


BEGIN_NAMESPACE_YM

class Grammer;
class LR0State;
class LR0Term;

//////////////////////////////////////////////////////////////////////
/// @class LR0Set LR0Set.h "LR0Set.h"
/// @brief LR(0)正準集を表すクラス
//////////////////////////////////////////////////////////////////////
class LR0Set
{
public:

  /// @brief コンストラクタ
  /// @param[in] grammer 元となる文法
  LR0Set(Grammer* grammer);

  /// @brief デストラクタ
  ~LR0Set();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態のリストを返す．
  const vector<LR0State*>&
  state_list() const;

  /// @brief 初期状態を返す．
  LR0State*
  start_state() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 状態を追加する．
  /// @param[in] grammer 元となる文法
  /// @param[in] terms 状態を表す項集合
  /// @return 対応する状態を返す．
  ///
  /// すでに等価は状態が存在したらその状態を返す．
  LR0State*
  new_state(Grammer* grammer,
	    const vector<LR0Term>& terms);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 状態のリスト
  vector<LR0State*> mStateList;

  // 初期状態
  LR0State* mStartState;

};

END_NAMESPACE_YM


#endif // LR0SET_H
