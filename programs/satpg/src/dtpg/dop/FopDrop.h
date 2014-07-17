#ifndef FOPDROP_H
#define FOPDROP_H

/// @file FopDrop.h
/// @brief FopDrop のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "FsimOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class FopDrop FopDrop.h "FopDrop.h"
/// @brief DopDrop 用の FsimOp
//////////////////////////////////////////////////////////////////////
class FopDrop :
  public FsimOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr 故障マネージャ
  /// @param[in] fsim 故障シミュレータ
  FopDrop(FaultMgr& fmgr,
	  Fsim& fsim);

  /// @brief デストラクタ
  virtual
  ~FopDrop();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 故障マネージャ
  FaultMgr& mMgr;

  // 故障シミュレータ
  Fsim& mFsim;

};

END_NAMESPACE_YM_SATPG


#endif // FOPDROP_H
