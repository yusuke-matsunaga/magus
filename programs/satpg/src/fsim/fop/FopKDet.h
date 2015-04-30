#ifndef FOPKDET_H
#define FOPKDET_H

/// @file FopKDet.h
/// @brief FopKDet のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013-2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FopKDet FopKDet.h "FopKDet.h"
/// @brief 定められた回数分のパタンを記録する FsimOp
//////////////////////////////////////////////////////////////////////
class FopKDet :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] f_list 故障のリスト
  /// @param[in] max_fault_id 故障IDの最大値+1
  FopKDet(Fsim& fsim,
	  const vector<const TpgFault*>& f_list,
	  ymuint max_fault_id);

  /// @brief デストラクタ
  virtual
  ~FopKDet();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  /// @param[in] dpat 検出したパタンを表すビットベクタ
  virtual
  void
  operator()(const TpgFault* f,
	     PackedVal dpat);

  /// @brief 検出回数をクリアする．
  void
  clear_count();

  /// @brief 検出回数のしきい値をセットする．
  void
  set_limit(ymuint limit);

  /// @brief パタンをセットする．
  void
  set_pattern(const vector<TestVector*>& pat_list);

  /// @brief 故障に対するパタンのリストを返す．
  /// @param[in] f_id 故障の ID
  const vector<TestVector*>&
  pat_list(ymuint f_id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シミュレータ
  Fsim& mFsim;

  // しきい値
  ymuint32 mLimit;

  // 現在のパタンリスト
  vector<TestVector*> mCurPatList;

  // 個々の故障を検出するパタンのリストの配列
  // 故障のIDでインデクシングされている．
  vector<vector<TestVector*> > mPatListArray;

};

END_NAMESPACE_YM_SATPG

#endif // FOPKDET_H
