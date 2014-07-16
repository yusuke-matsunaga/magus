#ifndef TIMECMD_H
#define TIMECMD_H

/// @file TimeCmd.h
/// @brief TimeCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.

// 時間計測を行うパッケージ
// 実はこのパッケージは Magus には依存していないので汎用である．


#include "tclpp/TclCmd.h"
#include "utils/StopWatch.h"
#include "satpg_nsdef.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
/// @class StopwatchObj TimeCmd.h "TimeCmd.h"
/// @brief 時間計測を行なうコマンド用のクラス
//////////////////////////////////////////////////////////////////////
class StopwatchObj :
  public TclCmd
{
public:

  // コンストラクタ
  StopwatchObj();

  // デストラクタ
  virtual
  ~StopwatchObj();


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
class StopwatchCls :
  public TclCmdCls
{
protected:

  // StopwatchObjのオブジェクトを生成する仮想関数
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

END_NAMESPACE_YM_SATPG

#endif // TIMECMD_H
