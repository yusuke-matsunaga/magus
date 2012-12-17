#ifndef OP1_H
#define OP1_H

/// @file atpg/src/dtpg/Op1.h
/// @brief Op1 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "DtpgOperator.h"


BEGIN_NAMESPACE_YM_SATPG_DTPG

//////////////////////////////////////////////////////////////////////
// DtpgSat 用の DtpgOperator
//////////////////////////////////////////////////////////////////////
class Op1 :
  public DtpgOperator
{
public:

  /// @brief コンストラクタ
  Op1(FaultMgr& fmgr,
      TvMgr& tvmgr);

  /// @brief デストラクタ
  ~Op1();


public:
  //////////////////////////////////////////////////////////////////////
  // DtpgOperator の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった場合に呼ばれる関数
  virtual
  void
  set_detected(DtpgFault* f,
	       const vector<DtpgNode*>& input_list,
	       const vector<Bool3>& model);

  /// @brief 検出不能のときに呼ばれる関数
  virtual
  void
  set_untestable(DtpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  FaultMgr& mFaultMgr;

  TvMgr& mTvMgr;

};

END_NAMESPACE_YM_SATPG_DTPG

#endif // OP1_H
