#ifndef OPTTESTCMD_H
#define OPTTESTCMD_H

/// @file OptTestCmd.h
/// @brief OptTestCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

// コマンドの製作例


#include "tclpp/TclCmd.h"


BEGIN_NAMESPACE_YM_TCLPP

//////////////////////////////////////////////////////////////////////
// オプション解析のテスト用コマンド
//////////////////////////////////////////////////////////////////////
class OptTestCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  OptTestCmd();

  /// @brief デストラクタ
  virtual
  ~OptTestCmd();


protected:

  // コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  TclPopt* mOptSwitch1;

  TclPoptStr* mOptStr1;

  TclPoptBool* mOptBool1;

  TclPoptInt* mOptInt1;

  TclPoptDouble* mOptDouble1;

  TclPopt* mOptA;

  TclPopt* mOptB;

  TclPopt* mOptC;

};

END_NAMESPACE_YM_TCLPP

#endif // OPTTESTCMD_H
