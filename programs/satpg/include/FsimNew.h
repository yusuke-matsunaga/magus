#ifndef FSIMNEW_H
#define FSIMNEW_H

/// @file FsimNew.h
/// @brief FsimNew のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012-2014 Yusuke Matsunaga
/// All rights reserved.


#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FsimNew FsimNew.h "FsimNew.h"
/// @brief 故障シミュレーションを行うクラスの基底クラス
//////////////////////////////////////////////////////////////////////
class FsimNew
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
  set_network(const TpgNetwork& network) = 0;

  /// @brief 故障にスキップマークをつける．
  virtual
  void
  set_skip(TpgFault* f) = 0;

  /// @brief SPSFP故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] f 対象の故障
  /// @retval true 故障の検出が行えた．
  /// @retval false 故障の検出が行えなかった．
  virtual
  bool
  spsfp(TestVector* tv,
	TpgFault* f) = 0;

  /// @brief ひとつのパタンで故障シミュレーションを行う．
  /// @param[in] tv テストベクタ
  /// @param[in] fault_list 対象の故障のリスト
  /// @param[in] det_faults このパタンで検出された故障のリスト
  virtual
  void
  sppfp(TestVector* tv,
	const vector<TpgFault*>& fault_list,
	vector<TpgFault*>& det_faults) = 0;

  /// @brief 複数のパタンで故障シミュレーションを行う．
  /// @param[in] tv_array テストベクタの配列
  /// @param[in] fault_list 対象の故障のリスト
  /// @param[in] op 検出した時に起動されるファンクタオブジェクト(Type2)
  virtual
  void
  ppsfp(const vector<TestVector*>& tv_array,
	const vector<TpgFault*>& fault_list,
	FsimOp& op) = 0;

};


//////////////////////////////////////////////////////////////////////
// Fsim の派生クラスのインスタンスを生成する関数
//////////////////////////////////////////////////////////////////////

/// @brief 2値版の故障シミュレータ
extern
FsimNew*
new_FsimNew2();

extern
FsimNew*
new_FsimNewX2();

END_NAMESPACE_YM_SATPG

#endif // FSIMNEW_H
