#ifndef MINPAT_H
#define MINPAT_H

/// @file MinPat.h
/// @brief MinPat のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPat MinPat.h "MinPat.h"
/// @brief テストベクタの最小化を行うクラス
//////////////////////////////////////////////////////////////////////
class MinPat
{
public:

  /// @brief デストラクタ
  virtual
  ~MinPat() {}


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストベクタの最小化を行なう．
  /// @param[in] network 対象のネットワーク
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim2 2値の故障シミュレータ(検証用)
  /// @param[out] tv_list テストベクタのリスト
  /// @param[out] stats 実行結果の情報を格納する変数
  virtual
  void
  run(TpgNetwork& network,
      TvMgr& tvmgr,
      FaultMgr& fmgr,
      Fsim& fsim2,
      vector<TestVector*>& tv_list,
      MinPatStats& stats) = 0;

  /// @brief verbose フラグをセットする．
  virtual
  void
  set_verbose(int verbose) = 0;

};


/// @brief インスタンスを生成する関数
// @param[in] group_dominance グループ支配を計算する．
// @param[in] fault_dominace 故障支配を計算する．
extern
MinPat*
new_MinPat(bool group_dominance,
	   bool fault_dominance);

/// @brief インスタンスを生成する関数
// @param[in] group_dominance グループ支配を計算する．
extern
MinPat*
new_MinPat2(bool group_dominance);

/// @brief インスタンスを生成する関数
extern
MinPat*
new_MinPatDsatur();

/// @brief インスタンスを生成する関数
extern
MinPat*
new_MinPatDsatur2();

END_NAMESPACE_YM_SATPG

#endif // MINPAT_H
