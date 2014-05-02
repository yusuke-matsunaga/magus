#ifndef KDET_H
#define KDET_H

/// @file KDet.h
/// @brief KDet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"
#include "KDetOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class KDet KDet.h "KDet.h"
/// @brief k-detect patterns を求めるクラス
//////////////////////////////////////////////////////////////////////
class KDet
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsm 故障シミュレータ
  /// @param[in] f_list 故障のリスト
  /// @param[in] max_fault_id 故障IDの最大値+1
  KDet(Fsim& fsim,
       const vector<TpgFault*>& f_list,
       ymuint max_fault_id);

  /// @brief デストラクタ
  ~KDet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 各故障に対して k 回検出するまでは故障シミュレーションを行う．
  /// @param[in] pat_list パタンのリスト
  /// @param[in] k_limit 検出回数のしきい値
  /// @param[in] det_list_array 各パタンが検出する故障番号のリストの配列
  void
  run(const vector<TestVector*>& pat_list,
      ymuint k_limit,
      vector<vector<ymuint> >& det_list_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // 故障検出時に呼ばれるファンクタ
  KDetOp mOp;

};

END_NAMESPACE_YM_SATPG


#endif // KDET_H
