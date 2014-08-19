#ifndef SRC_DTPG_DTPGOLDCMD_H
#define SRC_DTPG_DTPGOLDCMD_H

/// @file src/atpg_tclsh/DtpgOldCmd.h
/// @brief DtpgOldCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////
class DtpgOldCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  DtpgOldCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DtpgOldCmd();


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

  // ffr オプションの解析用オブジェクト
  TclPopt* mPoptFFR;

  // mffc オプションの解析用オブジェクト
  TclPopt* mPoptMFFC;

  // all オプションの解析用オブジェクト
  TclPopt* mPoptAll;

  // po オプションの解析用オブジェクト
  TclPopt* mPoptPo;

  // rpo オプションの解析用オブジェクト
  TclPopt* mPoptRpo;

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

#endif // SRC_DTPG_DTPGOLDCMD_H
