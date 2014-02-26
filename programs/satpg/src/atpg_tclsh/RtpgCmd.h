#ifndef RTPGCMD_H
#define RTPGCMD_H

/// @file RtpgCmd.h
/// @brief RtpgCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 故障シミュレーションを実行するコマンド
//////////////////////////////////////////////////////////////////////
class RtpgCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  RtpgCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~RtpgCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // npat オプションの解析用オブジェクト
  TclPoptInt* mPoptNpat;

  // min_fault オプションの解析用オブジェクト
  TclPoptInt* mPoptMin;

  // max_interval オプションの解析用オブジェクト
  TclPoptInt* mPoptMax;

  // seed オプションの解析用オブジェクト
  TclPoptUint* mPoptSeed;

  // file オプションの解析用オブジェクト
  TclPoptStr* mPoptFile;

  // print_stats オプションの解析用オブジェクト
  TclPopt* mPoptPrintStats;

  // old オプションの解析用オブジェクト
  TclPopt* mPoptOld;

};

END_NAMESPACE_YM_SATPG

#endif // RTPGCMD_H
