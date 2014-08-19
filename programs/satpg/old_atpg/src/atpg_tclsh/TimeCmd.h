﻿#ifndef ATPG_MAIN_TIMECMD_H
#define ATPG_MAIN_TIMECMD_H

/// @file atpg/src/misc/TimeCmd.h
/// @brief TimeCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TimeCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 時間計測を行うパッケージ
// 実はこのパッケージは Magus には依存していないので汎用である．


#include "ym_YmTclpp/TclCmd.h"
#include "ym_YmUtils/StopWatch.h"
#include "atpg_nsdef.h"


BEGIN_NAMESPACE_YM_ATPG

//////////////////////////////////////////////////////////////////////
// ストップウォッチ操作を行うコマンド用のクラス
//////////////////////////////////////////////////////////////////////
class StpwatchObj :
  public TclCmd
{
public:

  // コンストラクタ
  StpwatchObj();

  // デストラクタ
  virtual
  ~StpwatchObj();


protected:
  
  // コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 本当のストップウォッチオブジェクト
  StopWatch mStopWatch;

};


//////////////////////////////////////////////////////////////////////
// タイマーオブジェクトを生成するコマンドクラス
//////////////////////////////////////////////////////////////////////
class StpwatchCls :
  public TclCmdCls
{
protected:

  // StpwatchObjのオブジェクトを生成する仮想関数
  virtual
  TclCmd*
  create_obj() const;

};


//////////////////////////////////////////////////////////////////////
// 時間計測コマンド
//////////////////////////////////////////////////////////////////////
class TimeCmd :
  public TclCmd
{
public:

  // コンストラクタ
  TimeCmd();

  // デストラクタ
  virtual
  ~TimeCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);
  

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 時間計測を行うオブジェクト
  StopWatch mStopWatch;

  // 今までの経過時間
  USTime mTotalTime;
};

END_NAMESPACE_YM_ATPG

#endif // ATPG_MAIN_TIMECMD_H
