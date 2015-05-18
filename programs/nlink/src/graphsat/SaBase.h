#ifndef SABASE_H
#define SABASE_H

/// @file SaBase.h
/// @brief SaBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "SatAnalyzer.h"


BEGIN_NAMESPACE_YM_NLINK

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
  /// @param[in] solver SATソルバ
  SaBase(GraphSat* solver);

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

  /// @brief lit_list を極小セパレータにする．
  /// @param[inout] lit_list リテラルのリスト
  ///
  /// lit_list から lit_list の他のリテラルの割り当て結果によって
  /// 割り当てられたリテラルを取り除く．
  void
  make_minimal(vector<Literal>& lit_list);

  /// @brief リテラルの並び替えを行う．
  /// @param[in] lit_list リテラルのリスト
  /// @return 2番めのリテラル (lit_list[1]) のレベルを返す．
  ///
  /// lit_list[0] は不変で，lit_list[1] 以降のリテラルで
  /// 最も decision level の高いリテラルを lit_list[1]
  /// に持ってくる．
  int
  reorder(vector<Literal>& lit_list);

  /// @brief mClearQueue につまれた変数のマークを消す．
  void
  clear_marks();

  /// @brief 変数のマークにアクセスする．
  /// @param[in] var 対象の変数
  bool
  get_mark(VarId var);

  /// @brief 変数にマークをつけてキューに積む
  /// @param[in] var 対象の変数
  void
  set_mark_and_putq(VarId var);

  /// @brief 変数のマークをセットする．
  /// @param[in] var 対象の変数
  /// @param[in] mark 設定するマークの値
  void
  set_mark(VarId var,
	   bool mark);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief make_minimal のサブルーティン
  /// @param[in] var 対象の変数
  /// @param[in] lmask lit_list に含まれる変数の決定レベルのハッシュ値
  bool
  check_recur(VarId var,
	      ymuint64 lmask);

  /// @brief check_recur のサブルーティン
  /// @param[in] var 対象の変数
  ///
  /// var が未処理なら var_stack に積む．
  void
  put_var(Literal lit);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数のマークを消すためのキュー
  vector<VarId> mClearQueue;

  // 変数のマーク
  vector<bool> mMark;

  // check_recur() で用いられるスタック
  vector<VarId> mVarStack;

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

// @brief check_recur のサブルーティン
// @param[in] var 対象の変数
//
// var が未処理ならキューに積む．
inline
void
SaBase::put_var(Literal lit)
{
  VarId var = lit.varid();
  if ( !get_mark(var) && decision_level(var) > 0 ) {
    set_mark_and_putq(var);
    mVarStack.push_back(var);
  }
}

END_NAMESPACE_YM_NLINK

#endif // SABASE_H
