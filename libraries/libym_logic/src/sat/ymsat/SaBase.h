#ifndef SABASE_H
#define SABASE_H

/// @file SaBase.h
/// @brief SaBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "SatAnalyzer.h"


BEGIN_NAMESPACE_YM_SAT

//////////////////////////////////////////////////////////////////////
/// @class SaBase SaBase.h "SaBase.h"
/// @brief SatAnalyzer のベース実装クラス
///
/// 継承クラスに共通な実装を提供するためのクラス
//////////////////////////////////////////////////////////////////////
class SaBase :
  public SatAnalyzer
{
public:

  /// @brief コンストラクタ
  SaBase();

  /// @brief デストラクタ
  virtual
  ~SaBase();


public:
  //////////////////////////////////////////////////////////////////////
  // SatAnalyer の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 新しい変数が追加されたときに呼ばれる仮想関数
  virtual
  void
  alloc_var(ymuint size);


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスのための実装関数
  //////////////////////////////////////////////////////////////////////

  // learnt を極小セパレータにする．
  void
  simplify(vector<Literal>& learnt);

  // decision level の高いリテラルを 2番め (learnt[1] の位置) に持ってくる
  // 2番めのリテラルのレベルを返す．
  int
  reorder(vector<Literal>& learnt);

  // mClearQueue につまれた変数のマークを消す．
  void
  clear_marks();

  // 変数のマークにアクセスする．
  bool
  get_mark(VarId var);

  // var->mMark を設定してキューに積む
  void
  set_mark_and_putq(VarId var);

  // 変数のマークをセットする．
  void
  set_mark(VarId var,
	   bool mark);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  // simplify のサブルーティン
  bool
  check_recur(VarId var,
	      ymuint64 lmask);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // var->mMark を消すためのキュー
  vector<VarId> mClearQueue;

  // var->mMark を消すためのキュー(simplify_recur用)
  vector<VarId> mClearQueue2;

  // 変数のマーク
  vector<bool> mMark;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// 変数のマークにアクセスする．
inline
bool
SaBase::get_mark(VarId var)
{
  return mMark[var.val()];
}

// 変数のマークをセットする．
inline
void
SaBase::set_mark(VarId var,
		 bool mark)
{
  mMark[var.val()] = mark;
}

END_NAMESPACE_YM_SAT

#endif // SABASE_H
