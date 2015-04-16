#ifndef KDET2_H
#define KDET2_H

/// @file KDet2.h
/// @brief KDet2 のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "KDet2Op.h"
#include "FaultInfo.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class KDet2 KDet2.h "KDet2.h"
/// @brief k-detect patterns を求めるクラス
//////////////////////////////////////////////////////////////////////
class KDet2
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsm 故障シミュレータ
  /// @param[in] f_list 故障のリスト
  KDet2(Fsim& fsim,
       const vector<TpgFault*>& f_list);

  /// @brief デストラクタ
  ~KDet2();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 各故障に対して k 回検出するまでは故障シミュレーションを行う．
  /// @param[in] pat_list パタンのリスト
  /// @param[in] fault_info_array 故障ごとの情報を収める配列
  void
  run(const vector<TestVector*>& pat_list,
      vector<FaultInfo>& fault_info_array);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  void
  record_pat(const vector<ymuint>& det_list,
	     ymuint pat_id,
	     vector<FaultInfo>& fault_info_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 故障番号の最大値
  ymuint mMaxFaultId;

  // 故障検出時に呼ばれるファンクタ
  KDet2Op mOp;

};

END_NAMESPACE_YM_SATPG


#endif // KDET2_H
