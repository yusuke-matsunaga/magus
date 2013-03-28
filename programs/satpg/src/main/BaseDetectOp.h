#ifndef BASEDETECTOP_H
#define BASEDETECTOP_H

/// @file BaseDetectOp.h
/// @brief BaseDetectOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "DetectOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BaseDetectOp BaseDetectOp.h "BaseDetectOp.h"
/// @brief DetectOp の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class BaseDetectOp :
  public DetectOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr FaultMgr
  BaseDetectOp(FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~BaseDetectOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テストパタンが見つかった時の処理
  /// @param[in] f 故障
  /// @param[in] tv テストパタン
  virtual
  void
  operator()(TpgFault* f,
	     TestVector* tv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FaultMgr
  FaultMgr& mMgr;

};

END_NAMESPACE_YM_SATPG

#endif // BASEDETECTOP_H
