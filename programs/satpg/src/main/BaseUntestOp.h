#ifndef BASEUNTESTOP_H
#define BASEUNTESTOP_H

/// @file BaseUntestOp.h
/// @brief BaseUntestOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2013 Yusuke Matsunaga
/// All rights reserved.


#include "UntestOp.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class BaseUntestOp BaseUntestOp.h "BaseUntestOp.h"
/// @brief UntestOp の基本的な動作を行なうクラス
//////////////////////////////////////////////////////////////////////
class BaseUntestOp :
  public UntestOp
{
public:

  /// @brief コンストラクタ
  /// @param[in] fmgr FaultMgr
  BaseUntestOp(FaultMgr& fmgr);

  /// @brief デストラクタ
  virtual
  ~BaseUntestOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief テスト不能故障と判定された時の処理
  /// @param[in] f 故障
  virtual
  void
  operator()(TpgFault* f);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // FaultMgr
  FaultMgr& mMgr;

};

END_NAMESPACE_YM_SATPG

#endif // BASEUNTESTOP_H
