#ifndef DOPDROPNEW_H
#define DOPDROPNEW_H

/// @file DopDropNew.h
/// @brief DopDropNew のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DopDropNew DopDropNew.h "DopDropNew.h"
/// @brief 故障シミュレーションを行なった故障ドロップを行なうクラス
//////////////////////////////////////////////////////////////////////
class DopDropNew :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  DopDropNew(FaultMgr& fmgr,
	     FsimNew& fsim);

  /// @brief デストラクタ
  virtual
  ~DopDropNew();


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
  FsimNew& mFsim;

  // 検出された故障を入れるリスト
  vector<TpgFault*> mDetFaults;

};

END_NAMESPACE_YM_SATPG

#endif // DOPDROPNEW_H
