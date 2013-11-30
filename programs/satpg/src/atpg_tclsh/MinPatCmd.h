#ifndef MINPATCMD_H
#define MINPATCMD_H

/// @file src/atpg_tclsh/MinPatCmd.h
/// @brief MinPatCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////
class MinPatCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  MinPatCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~MinPatCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // print_stats オプションの解析用オブジェクト
  TclPopt* mPoptPrintStats;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATCMD_H
