#ifndef SKIPOP_H
#define SKIPOP_H

/// @file SkipOp.h
/// @brief SkipOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "TpgOperator.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class SkipOp SkipOp.h "SkipOp.h"
/// @brief スキップありの TpgOperator
//////////////////////////////////////////////////////////////////////
class SkipOp :
  public TpgOperator
{
public:

  /// @brief コンストラクタ
  SkipOp(FaultMgr& fault_mgr,
	 TvMgr& tvmgr,
	 vector<TestVector*>& tv_list,
	 Fsim& fsim3,
	 ymuint threshold,
	 bool drop = false,
	 bool verify = false);

  /// @brief デストラクタ
  ~SkipOp();


public:
  //////////////////////////////////////////////////////////////////////
  // TpgOperator の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  /// @param[in] f 故障
  /// @param[in] val_list "入力ノードの番号 x 2 + 値" のリスト
  virtual
  void
  set_detected(TpgFault* f,
	       const vector<ymuint>& val_list);

  /// @brief 検出不能のときに呼ばれる関数
  /// @param[in] f 故障
  virtual
  void
  set_untestable(TpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  FaultMgr& mFaultMgr;

  TvMgr& mTvMgr;

  vector<TestVector*>& mTvList;

  Fsim& mFsim3;

  bool mDrop;

  bool mVerify;

  ymuint mThreshold;

  vector<TpgFault*> mUntestList;

  vector<TpgFault*> mSkipList;

};

END_NAMESPACE_YM_SATPG

#endif // SKIPOP_H
