#ifndef FAULTCMD_H
#define FAULTCMD_H

/// @file src/atpg_tclsh/FaultCmd.h
/// @brief FaultCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障の設定を行うコマンド
//////////////////////////////////////////////////////////////////////
class SetFault :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  SetFault(AtpgMgr* mgr);


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_YM_SATPG

#endif // FAULTCMD_H
