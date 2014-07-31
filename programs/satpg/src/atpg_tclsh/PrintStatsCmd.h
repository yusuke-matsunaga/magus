#ifndef PRINTSTATSCMD_H
#define PRINTSTATSCMD_H

/// @file src/atpg_tclsh/PrintStatsCmd.h
/// @brief PrintStatsCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"
#include "YmUtils/StopWatch.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintStatsCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  PrintStatsCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PrintStatsCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 計時用のタイマー
  StopWatch mStopWatch;

};

END_NAMESPACE_YM_SATPG

#endif // PRINTSTATSCMD_H
