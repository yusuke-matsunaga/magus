#ifndef KDET2OP_H
#define KDET2OP_H

/// @file KDet2Op.h
/// @brief KDet2Op のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class KDet2Op KDet2Op.h "KDet2Op.h"
/// brief KDet2 用の FsimOp
//////////////////////////////////////////////////////////////////////
class KDet2Op :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fsim 故障シミュレータ
  /// @param[in] f_list 故障のリスト
  KDet2Op(Fsim& fsim,
	  const vector<TpgFault*>& f_list);

  /// @brief デストラクタ
  virtual
  ~KDet2Op();


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

  // 検出された故障番号のリスト
  vector<ymuint> mDetListArray[kPvBitLen];

};

END_NAMESPACE_YM_SATPG


#endif // KDET2OP_H
