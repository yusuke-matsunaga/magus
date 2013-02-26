#ifndef FSIM_H
#define FSIM_H

/// @file include/Fsim.h
/// @brief Fsim のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2013 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class Fsim Fsim.h "Fsim.h"
/// @brief 故障シミュレーションを行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class Fsim
{
public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ネットワークをセットする．
  virtual
  void
  set_network(const TpgNetwork& network) = 0;

  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[out] det_faults 検出された故障を格納するリスト
  virtual
  void
  run(TestVector* tv,
      vector<const TpgFault*>& det_faults) = 0;

  /// @brief 故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[out] det_faults 検出された故障を格納するリストの配列
  virtual
  void
  run(const vector<TestVector*>& tv_array,
      vector<vector<const TpgFault*> >& det_faults) = 0;

  /// @brief 一つのパタンで一つの故障に対するシミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  virtual
  bool
  run(TestVector* tv,
      const TpgFault* f) = 0;

};


extern
Fsim*
new_Fsim2();

extern
Fsim*
new_Fsim3();

extern
Fsim*
new_FsimX();

END_NAMESPACE_YM_SATPG

#endif // FSIM_H
