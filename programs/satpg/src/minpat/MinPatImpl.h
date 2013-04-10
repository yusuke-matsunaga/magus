#ifndef MINPATIMPL_H
#define MINPATIMPL_H

/// @file MinPatImpl.h
/// @brief MinPatImpl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "MinPat.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class MinPatImpl MinPatImpl.h "MinPatImpl.h"
/// @brief MinPat の実装クラス
//////////////////////////////////////////////////////////////////////
class MinPatImpl :
  public MinPat
{
public:

  /// @brief コンストラクタ
  /// @param[in] tvmgr テストベクタマネージャ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim2 2値の故障シミュレータ
  /// @param[in] fsim3 3値の故障シミュレータ
  MinPatImpl(TvMgr& tvmgr,
	     FaultMgr& fmgr,
	     Fsim& fsim2,
	     Fsim& fsim3);

  /// @brief デストラクタ
  virtual
  ~MinPatImpl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストベクタの最小化を行なう．
  /// @param[in] tv_list テストベクタのリスト
  /// @param[in] stats 実行結果の情報を格納する変数
  virtual
  void
  run(vector<TestVector*>& tv_list,
      MinPatStats& stats);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // テストベクタマネージャ
  TvMgr& mTvMgr;

  // 故障マネージャ
  FaultMgr& mFaultMgr;

  // 2値の故障シミュレータ
  Fsim& mFsim2;

  // 3値の故障シミュレータ
  Fsim& mFsim3;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATIMPL_H
