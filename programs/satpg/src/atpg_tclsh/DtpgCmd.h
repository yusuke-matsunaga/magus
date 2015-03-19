#ifndef DTPGCMD_H
#define DTPGCMD_H

/// @file DtpgCmd.h
/// @brief DtpgCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// テストパタン生成を行うコマンド
//////////////////////////////////////////////////////////////////////
class DtpgCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  DtpgCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DtpgCmd();


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

  // sat-option オプションの解析用オブジェクト
  TclPoptStr* mPoptSatOption;

  // satrec モードオプションの解析用オブジェクト
  TclPopt* mPoptSatRec;

  // minisat モードオプションの解析用オブジェクト
  TclPopt* mPoptMiniSat;

  // minisat2 モードオプションの解析用オブジェクト
  TclPopt* mPoptMiniSat2;

  // print_stats オプションの解析用オブジェクト
  TclPopt* mPoptPrintStats;

  // single オプションの解析用オブジェクト
  TclPopt* mPoptSingle;

  // single2 オプションの解析用オブジェクト
  TclPoptInt* mPoptSingle2;

  // single3 オプションの解析用オブジェクト
  TclPopt* mPoptSingle3;

  // multi オプションの解析用オブジェクト
  TclPopt* mPoptMulti;

  // multi2 オプションの解析用オブジェクト
  TclPoptInt* mPoptMulti2;

  // smt_single オプションの解析用オブジェクト
  TclPopt* mPoptSmtSingle;

  // forget オプションの解析用オブジェクト
  TclPopt* mPoptForget;

  // ffr オプションの解析用オブジェクト
  TclPopt* mPoptFFR;

  // mffc オプションの解析用オブジェクト
  TclPopt* mPoptMFFC;

  // po オプションの解析用オブジェクト
  TclPopt* mPoptPo;

  // rpo オプションの解析用オブジェクト
  TclPopt* mPoptRpo;

  // skip オプションの解析用オブジェクト
  TclPoptInt* mPoptSkip;

  // X抽出オプションの解析用オブジェクト
  TclPoptInt* mPoptX;

  // drop オプションの解析用オブジェクト
  TclPopt* mPoptDrop;

  // k_det オプションの解析用オブジェクト
  TclPoptInt* mPoptKDet;

  // オプション文字列オプションの解析用オブジェクト
  TclPoptStr* mPoptOpt;

  // verify オプションの解析用オブジェクト
  TclPopt* mPoptVerify;

  // timer オプションの解析用オブジェクト
  TclPopt* mPoptTimer;

  // no-timer オプションの解析用オブジェクト
  TclPopt* mPoptNoTimer;

};

END_NAMESPACE_YM_SATPG

#endif // DTPGCMD_H
