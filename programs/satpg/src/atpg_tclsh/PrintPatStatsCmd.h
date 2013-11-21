#ifndef PRINTPATSTATSCMD_H
#define PRINTPATSTATSCMD_H

/// @file src/atpg_tclsh/PrintPatStatsCmd.h
/// @brief PrintPatStatsCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// テストパタンの諸元を表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintPatStatsCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  PrintPatStatsCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PrintPatStatsCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // histgram オプション解析用のオブジェクト
  TclPopt* mPoptHist;

};

END_NAMESPACE_YM_SATPG

#endif // PRINTPATSATSCMD_H
