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
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim2 2値の故障シミュレータ
  /// @param[in] fsim3 3値の故障シミュレータ
  /// @param[inout] tv_list テストベクタのリスト
  /// @param[out] stats 実行結果の情報を格納する変数
  virtual
  void
  run(TvMgr& tvmgr,
      FaultMgr& fmgr,
      Fsim& fsim2,
      Fsim& fsim3,
      vector<TestVector*>& tv_list,
      MinPatStats& stats) = 0;

};


/// @brief インスタンスを生成する関数
extern
MinPat*
new_MinPat();

END_NAMESPACE_YM_SATPG

#endif // MINPAT_H
