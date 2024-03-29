﻿#ifndef MAGUS_MAIN_TIMECMD_H
#define MAGUS_MAIN_TIMECMD_H

/// @file magus/main/TimeCmd.h
/// @brief TimeCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TimeCmd.h 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 時間計測を行うパッケージ
// 実はこのパッケージは Magus には依存していないので汎用である．


#include "magus.h"

#include "ym/TclCmd.h"
#include "ym/Timer.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ストップウォッチ操作を行うコマンド用のクラス
//////////////////////////////////////////////////////////////////////
class TimerObj :
  public TclCmd
{
public:

  // コンストラクタ
  TimerObj();

  // デストラクタ
  virtual
  ~TimerObj();


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
  Timer mTimer;

};


//////////////////////////////////////////////////////////////////////
// タイマーオブジェクトを生成するコマンドクラス
//////////////////////////////////////////////////////////////////////
class StpwatchCls :
  public TclCmdCls
{
protected:

  // TimerObjのオブジェクトを生成する仮想関数
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
  Timer mTimer;

  // 今までの経過時間
  double mTotalTime;
};

END_NAMESPACE_MAGUS

#endif // MAGUS_MAIN_TIMECMD_H
