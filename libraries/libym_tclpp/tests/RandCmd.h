#ifndef RANDCMD_H
#define RANDCMD_H

/// @file RandCmd.h
/// @brief RandCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// 乱数を発生させるコマンドクラス


#include "tclpp/TclCmd.h"
#include "utils/RandGen.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// 乱数発生コマンド
//////////////////////////////////////////////////////////////////////
class RandCmd :
  public TclCmd
{
protected:

  // usage を出力する関数
  virtual
  void
  usage();

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 乱数発生器
  RandGen mRandGen;

};

END_NAMESPACE_YM_TCLPP

#endif // RANDCMD_H
