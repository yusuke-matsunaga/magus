#ifndef LNMATCH_H
#define LNMATCH_H

/// @file LnMatch.h
/// @brief LnMatch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "RcfNetwork.h"
#include "YmLogic/TvFunc.h"
#include "GbmSolver.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class LnMatch LnMatch.h "LnMatch.h"
/// @brief LUT ネットワークに対するマッチングを行うクラス
//////////////////////////////////////////////////////////////////////
class LnMatch
{
public:

  /// @brief コンストラクタ
  LnMatch();

  /// @brief デストラクタ
  ~LnMatch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 与えられた関数を LUT ネットワークで実現できるか調べる．
  /// @param[in] func 対象の関数
  /// @param[in] solver GBM ソルバ
  bool
  match(const TvFunc& func,
	GbmSolver& solver);

  /// @brief カウンタをリセットする．
  void
  reset_count();

  /// @brief テンプレート数を得る．
  ymuint
  templ_num() const;

  /// @brief テンプレートのカウンタ値を得る．
  /// @param[in] pos テンプレート番号 ( 0 <= pos < templ_num() )
  ymuint
  get_templ_count(ymuint pos) const;

  /// @brief テンプレートのループカウンタ値を得る．
  /// @param[in] pos テンプレート番号 ( 0 <= pos < templ_num() )
  ymuint
  get_templ_loop_count(ymuint pos) const;

  /// @brief カウンタの値を得る．
  void
  get_count(ymuint& trivial_num,
	    ymuint& fail_num);


public:
  //////////////////////////////////////////////////////////////////////
  // テンプレートを登録する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 入力数の範囲を指定する．
  void
  set_input_range(ymuint min_inputs,
		  ymuint max_inputs);

  /// @brief テンプレートを追加する．
  void
  add_templ(ymuint input_num,
	    RcfNetwork* network);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 内部のデータを消去する．
  void
  clear();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられるデータ構造
  //////////////////////////////////////////////////////////////////////

  struct RcfTempl
  {
    // LUT ネットワーク
    RcfNetwork* mNetwork;

    // マッチした回数
    ymuint32 mCount;

    // マッチ中のループ回数
    ymuint64 mLoopCount;

  };


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // マッチングを行う最小の入力数
  ymuint32 mMinInputs;

  // マッチングを行う最大の入力数
  ymuint32 mMaxInputs;

  // 全テンプレートのリスト
  vector<RcfTempl*> mAllList;

  // 入力数ごとのマッチングテンプレートのリスト
  vector<vector<RcfTempl*> > mTemplList;

  // 自明なマッチ (<= 5)の回数
  ymuint32 mTrivialNum;

  // マッチしなかった回数
  ymuint32 mFailNum;

};

END_NAMESPACE_YM

#endif // LNMATCH_H
