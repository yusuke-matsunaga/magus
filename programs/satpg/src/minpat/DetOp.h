#ifndef DETOP_H
#define DETOP_H

/// @file DetOp.h
/// @brief DetOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class DetOp DetOp.h "DetOp.h"
/// brief MinPat 用の FsimOp
//////////////////////////////////////////////////////////////////////
class DetOp :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  DetOp();

  /// @brief デストラクタ
  virtual
  ~DetOp();


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

  /// @brief 検出された故障のID番号とビットベクタの対のリストを返す．
  const vector<pair<ymuint, PackedVal> >&
  det_list();

  /// @brief det_list をクリアする．
  void
  clear_det_list();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 検出された故障番号とビットベクタの対のリスト
  vector<pair<ymuint, PackedVal> > mDetList;

};

END_NAMESPACE_YM_SATPG


#endif // DETOP_H
