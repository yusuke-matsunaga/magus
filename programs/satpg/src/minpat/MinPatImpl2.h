#ifndef MINPATIMPL2_H
#define MINPATIMPL2_H

/// @file MinPatImpl2.h
/// @brief MinPatImpl2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat.h"
#include "NodeValList.h"
#include "FaultInfo.h"
#include "NodeSet.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatImpl2 MinPatImpl2.h "MinPatImpl2.h"
/// @brief MinPat の実装クラス
//////////////////////////////////////////////////////////////////////
class MinPatImpl2 :
  public MinPat
{
public:

  /// @brief コンストラクタ
  MinPatImpl2();

  /// @brief デストラクタ
  virtual
  ~MinPatImpl2();


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
      MinPatStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

};

END_NAMESPACE_YM_SATPG

#endif // MINPATIMPL2_H
