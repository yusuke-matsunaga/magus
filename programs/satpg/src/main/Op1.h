#ifndef OP1_H
#define OP1_H

/// @file atpg/src/main/Op1.h
/// @brief Op1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "TpgOperator.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// DtpgSat 用の DtpgOperator
//////////////////////////////////////////////////////////////////////
class Op1 :
  public TpgOperator
{
public:

  /// @brief コンストラクタ
  Op1(TvMgr& tvmgr,
      vector<TestVector*>& tv_list,
      Fsim& fsim3,
      bool drop = false,
      bool verify = false);

  /// @brief デストラクタ
  ~Op1();


public:
  //////////////////////////////////////////////////////////////////////
  // DtpgOperator の仮想関数
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

  TvMgr& mTvMgr;

  vector<TestVector*>& mTvList;

  Fsim& mFsim3;

  bool mDrop;

  bool mVerify;
};

END_NAMESPACE_YM_SATPG

#endif // OP1_H
