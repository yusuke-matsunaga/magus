
/// @file magus/main/TimeCmd.cc
/// @brief TimeCmd の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: TimeCmd.cc 1417 2008-05-06 18:12:40Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.

// 時間計測を行うパッケージ
// 実はこのパッケージは Magus には依存していないので汎用になる．


#include "TimeCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
// ストップウォッチ操作を行うコマンド用のクラス
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TimerObj::TimerObj()
{
  const char* usage =
    " CMD\n"
    "\treset;     # reset the stopwatch\n"
    "\tstart;     # start the stopwatch\n"
    "\tstop;      # stop the stopwatch\n"
    "\tget;       # get real/user/system time\n"
    "\tget_rtime; # get real time\n"
    "\tget_utime; # get user time\n"
    "\tget_stime; # get system time";
  set_usage_string(usage);
}

// デストラクタ
TimerObj::~TimerObj()
{
}

// ストップウォッチを操作するコマンド
int
TimerObj::cmd_proc(TclObjVector& objv)
{
  if ( objv.size() != 2 ) {
    print_usage();
    return TCL_ERROR;
  }

  string cmd = objv[1];
  if ( cmd == "reset" ) {
    mTimer.reset();
  }
  else if ( cmd == "start" ) {
    mTimer.start();
  }
  else if ( cmd == "stop" ) {
    mTimer.stop();
  }
  else if ( cmd == "get" ) {
    auto time = mTimer.get_time();
    TclObj result(time);
    set_result(result);
  }
  else {
    TclObj emsg;
    emsg << cmd << " : Bad stopwatch command";
    set_result(emsg);
    return TCL_ERROR;
  }
  return TCL_OK;
}


//////////////////////////////////////////////////////////////////////
// タイマーオブジェクトを生成するコマンドクラス
//////////////////////////////////////////////////////////////////////

// TimerObjのオブジェクトを生成する仮想関数
TclCmd*
StpwatchCls::create_obj() const
{
  return new TimerObj();
}


//////////////////////////////////////////////////////////////////////
// 時間計測コマンド
//////////////////////////////////////////////////////////////////////

// コンストラクタ
TimeCmd::TimeCmd()
{
  mTimer.reset();
  mTimer.start();
  mTotalTime = 0.0;
}

// デストラクタ
TimeCmd::~TimeCmd()
{
}

// コマンド処理関数
// 引数がなければ，前回このコマンドが呼ばれてからの経過時間を計測する．
// 引数があれば残りの引数を実行し，その実行時間を計測する．
int
TimeCmd::cmd_proc(TclObjVector& objv)
{
  mTimer.stop();
  auto time = mTimer.get_time();
  mTotalTime += time;

  int code = TCL_OK;
  TclObj res;
  if ( objv.size() > 1 ) {
    mTimer.reset();
    mTimer.start();

    // 残りの引数を評価する．
    TclObjVector::const_iterator p1 = objv.begin();
    p1 ++;
    TclObjVector::const_iterator p2 = objv.end();
    TclObjVector tmp_array(p1, p2);
    TclObj new_args(tmp_array);
    code = eval(new_args);

    mTimer.stop();
    time = mTimer.get_time();
    mTotalTime += time;
    res = result();
    res << "\n";
  }

  char buff[128];
  sprintf(buff, "Lap:%7.2fms, Total:%7.2fms",
	  time,
	  mTotalTime);

  mTimer.reset();
  mTimer.start();

  res << buff;
  set_result(res);

  return code;
}

END_NAMESPACE_MAGUS
