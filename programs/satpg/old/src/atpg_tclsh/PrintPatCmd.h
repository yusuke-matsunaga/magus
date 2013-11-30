#ifndef PRINTPATCMD_H
#define PRINTPATCMD_H

/// @file src/atpg_tclsh/PrintPatCmd.h
/// @brief PrintPatCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// テストパタンを表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintPatCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  PrintPatCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PrintPatCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // hex オプションの解析用オブジェクト
  TclPopt* mPoptHex;

  // num オプションの解析用オブジェクト
  TclPopt* mPoptNum;

};

END_NAMESPACE_YM_SATPG

#endif // PRINTPATCMD_H
