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
  /// @param[in] network ネットワーク
  /// @param[in] fault_mgr 故障マネージャ
  virtual
  void
  set_network(const TpgNetwork& network,
	      FaultMgr& fault_mgr) = 0;

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] dop_list DetectOp のリスト
  virtual
  void
  sppfp(TestVector* tv,
	const vector<DetectOp*>& dop_list) = 0;

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] dop_list DetectOp のリスト
  virtual
  void
  ppsfp(const vector<TestVector*>& tv_array,
	const vector<DetectOp*>& dop_list) = 0;

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(TestVector* tv,
	TpgFault* f) = 0;

};


//////////////////////////////////////////////////////////////////////
// Fsim の派生クラスのインスタンスを生成する関数
//////////////////////////////////////////////////////////////////////

extern
Fsim*
new_Fsim2();

extern
Fsim*
new_Fsim3();

extern
Fsim*
new_FsimX();

extern
Fsim*
new_FsimX2();

END_NAMESPACE_YM_SATPG

#endif // FSIM_H
