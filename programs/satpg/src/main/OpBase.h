#ifndef OPBASE_H
#define OPBASE_H

/// @file OpBase.h
/// @brief OpBase のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "TpgOperator.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class OpBase OpBase.h "OpBase.h"
/// @brief TpgOperator のベースクラス
//////////////////////////////////////////////////////////////////////
class OpBase :
  public TpgOperator
{
public:

  /// @brief コンストラクタ
  OpBase(FaultMgr& fault_mgr,
	 TvMgr& tvmgr,
	 vector<TestVector*>& tv_list,
	 Fsim& fsim3,
	 bool drop = false,
	 bool verify = false);

  /// @brief デストラクタ
  ~OpBase();


public:
  //////////////////////////////////////////////////////////////////////
  // TpgOperator の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  /// @param[in] f 故障
  /// @param[in] val_list ("入力ノードの番号 x 3 + 値") のリスト
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

  // 故障を管理するクラス
  FaultMgr& mFaultMgr;

  // テストベクタを管理するクラス
  TvMgr& mTvMgr;

  // テストベクタのリスト
  vector<TestVector*>& mTvList;

  Fsim& mFsim3;

  bool mDrop;

  bool mVerify;
};

END_NAMESPACE_YM_SATPG

#endif // OPBASE_H
