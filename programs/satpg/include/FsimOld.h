#ifndef FSIMOLD_H
#define FSIMOLD_H

/// @file FsimOld.h
/// @brief FsimOld のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FsimOld FsimOld.h "FsimOld.h"
/// @brief 故障シミュレーションを行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class FsimOld
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

  /// @brief SPPFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] det_faults 検出された故障を格納するリスト
  virtual
  void
  sppfp(TestVector* tv,
	vector<TpgFault*>& det_faults) = 0;

  /// @brief PPSFP故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] det_faults 検出された故障を格納するリストの配列
  virtual
  void
  ppsfp(const vector<TestVector*>& tv_array,
	vector<vector<TpgFault*> >& det_faults) = 0;

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
// FsimOld の派生クラスのインスタンスを生成する関数
//////////////////////////////////////////////////////////////////////

extern
FsimOld*
new_FsimOld2();

extern
FsimOld*
new_FsimOld3();

extern
FsimOld*
new_FsimOldX();

extern
FsimOld*
new_FsimOldX2();

END_NAMESPACE_YM_SATPG

#endif // FSIMOLD_H
