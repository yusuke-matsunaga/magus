#ifndef DROPDETECTOP_H
#define DROPDETECTOP_H

/// @file DropDetectOp.h
/// @brief DropDetectOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DropDetectOp DropDetectOp.h "DropDetectOp.h"
/// @brief 故障シミュレーションを行なった故障ドロップを行なうクラス
//////////////////////////////////////////////////////////////////////
class DropDetectOp :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  DropDetectOp(FaultMgr& fmgr,
	       Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~DropDetectOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] tv テストパタン
  virtual
  void
  operator()(TpgFault* f,
	     TestVector* tv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障マネージャ
  FaultMgr& mMgr;

  // 故障シミュレータ
  Fsim& mFsim;

  // 検出された故障を入れるリスト
  vector<TpgFault*> mDetFaults;

};

END_NAMESPACE_YM_SATPG

#endif // DROPDETECTOP_H
