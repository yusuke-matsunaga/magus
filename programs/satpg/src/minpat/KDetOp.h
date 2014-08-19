#ifndef KDETOP_H
#define KDETOP_H

/// @file KDetOp.h
/// @brief KDetOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class KDetOp KDetOp.h "KDetOp.h"
/// brief KDet 用の FsimOp
//////////////////////////////////////////////////////////////////////
class KDetOp :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] f_list 故障のリスト
  /// @param[in] max_fault_id 故障IDの最大値+1
  KDetOp(Fsim& fsim,
	 const vector<TpgFault*>& f_list,
	 ymuint max_fault_id);

  /// @brief デストラクタ
  virtual
  ~KDetOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 故障を検出したときの処理
  /// @param[in] f 故障
  /// @param[in] dpat 検出したパタンを表すビットベクタ
  virtual
  void
  operator()(TpgFault* f,
	     PackedVal dpat);

  /// @brief 検出回数をクリアする．
  void
  clear_count();

  /// @brief 検出回数のしきい値をセットする．
  void
  set_limit(ymuint limit);

  /// @brief 検出された故障のID番号のリストを返す．
  const vector<ymuint>&
  det_list(ymuint pos);

  /// @brief det_list をクリアする．
  void
  clear_det_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障シュミレータ
  Fsim& mFsim;

  // しきい値
  ymuint32 mLimit;

  // 各故障の検出回数
  vector<ymuint> mDetCount;

  // 検出された故障番号のリスト
  vector<ymuint> mDetListArray[kPvBitLen];

};

END_NAMESPACE_YM_SATPG


#endif // KDETOP_H
