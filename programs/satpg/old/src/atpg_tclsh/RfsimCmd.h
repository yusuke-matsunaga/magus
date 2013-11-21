#ifndef RFSIMCMD_H
#define RFSIMCMD_H

/// @file src/atpg_tclsh/RfsimCmd.h
/// @brief RfsimCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 逆順故障シミュレーションを行うコマンド
//////////////////////////////////////////////////////////////////////
class RfsimCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  RfsimCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~RfsimCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 逆順故障シミュレーションを行うクラス
  //Rfsim mRfsim;

};

END_NAMESPACE_YM_SATPG

#endif // RFSIMCMD_H
