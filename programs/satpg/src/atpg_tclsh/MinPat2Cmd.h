#ifndef MINPAT2CMD_H
#define MINPAT2CMD_H

/// @file MinPat2Cmd.h
/// @brief MinPat2Cmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////
class MinPat2Cmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  MinPat2Cmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~MinPat2Cmd();


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

#endif // MINPAT2CMD_H
