#ifndef NAIVEMINPAT_H
#define NAIVEMINPAT_H

/// @file NaiveMinPat.h
/// @brief NaiveMinPat のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class NaiveMinPat NaiveMinPat.h "NaiveMinPat.h"
/// @brief MinPat の実装クラス
//////////////////////////////////////////////////////////////////////
class NaiveMinPat :
  public MinPat
{
public:

  /// @brief コンストラクタ
  NaiveMinPat();

  /// @brief デストラクタ
  virtual
  ~NaiveMinPat();


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
      MinPatStats& stats);

};

END_NAMESPACE_YM_SATPG

#endif // NAIVEMINPAT_H
