#ifndef DTPG2CMD_H
#define DTPG2CMD_H

/// @file Dtpg2Cmd.h
/// @brief Dtpg2Cmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////
class Dtpg2Cmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  Dtpg2Cmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~Dtpg2Cmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // verbose オプションの解析用オブジェクト
  TclPoptInt* mPoptVerbose;

  // sat モードオプションの解析用オブジェクト
  TclPopt* mPoptSat;

  // satrec モードオプションの解析用オブジェクト
  TclPopt* mPoptSatRec;

  // minisat モードオプションの解析用オブジェクト
  TclPopt* mPoptMiniSat;

  // fsim モードオプションの解析用オブジェクト
  TclPoptStr* mPoptFsim;

  // print_stats オプションの解析用オブジェクト
  TclPopt* mPoptPrintStats;

  // single オプションの解析用オブジェクト
  TclPopt* mPoptSingle;

  // dual オプションの解析用オブジェクト
  TclPopt* mPoptDual;

  // node オプションの解析用オブジェクト
  TclPopt* mPoptNode;

  // ffr オプションの解析用オブジェクト
  TclPopt* mPoptFFR;

  // mffc オプションの解析用オブジェクト
  TclPopt* mPoptMFFC;

  // all オプションの解析用オブジェクト
  TclPopt* mPoptAll;

  // po オプションの解析用オブジェクト
  TclPopt* mPoptPo;

  // skip オプションの解析用オブジェクト
  TclPopt* mPoptSkip;

  // X抽出オプションの解析用オブジェクト
  TclPoptInt* mPoptX;

  // dry runオプションの解析用オブジェクト
  TclPopt* mPoptDryRun;

  // drop オプションの解析用オブジェクト
  TclPopt* mPoptDrop;

  // verify オプションの解析用オブジェクト
  TclPopt* mPoptVerify;

  // timer オプションの解析用オブジェクト
  TclPopt* mPoptTimer;

};

END_NAMESPACE_YM_SATPG

#endif // DTPG2CMD_H
