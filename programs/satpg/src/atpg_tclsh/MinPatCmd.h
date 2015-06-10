#ifndef MINPATCMD_H
#define MINPATCMD_H

/// @file MinPatCmd.h
/// @brief MinPatCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
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

  // simple オプション
  TclPopt* mPoptSimple;

  // simple2 オプション
  TclPopt* mPoptSimple2;

  // dsatur オプション
  TclPopt* mPoptDsatur;

  // dsatur2 オプション
  TclPopt* mPoptDsatur2;

  // group-dominance オプション
  TclPopt* mPoptGroupDominance;

  // fault-dominance オプション
  TclPopt* mPoptFaultDominance;

  // print_stats オプションの解析用オブジェクト
  TclPopt* mPoptPrintStats;

  // verbose オプションの解析用オブジェクト
  TclPoptInt* mPoptVerbose;

  // exact オプションの解析用オブジェクト
  TclPopt* mPoptExact;

  // compaction オプションの解析用オブジェクト
  TclPopt* mPoptCompaction;

  // fast-compaction オプションの解析用オブジェクト
  TclPopt* mPoptFastCompaction;

  // mc-compaction オプションの解析用オブジェクト
  TclPopt* mPoptMcCompaction;

  // thval オプションの解析用オブジェクト
  TclPoptInt* mPoptThVal;

  // rep-faults オプションの解析用オブジェクト
  TclPopt* mPoptRepFaults;

};

END_NAMESPACE_YM_SATPG

#endif // MINPATCMD_H
