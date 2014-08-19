#ifndef PRINTFAULTCMD_H
#define PRINTFAULTCMD_H

/// @file PrintFaultCmd.h
/// @brief PrintFaultCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AtpgCmd.h"


BEGIN_NAMESPACE_YM_SATPG

//////////////////////////////////////////////////////////////////////
// 情報を表示するコマンド
//////////////////////////////////////////////////////////////////////
class PrintFaultCmd :
  public AtpgCmd
{
public:

  /// @brief コンストラクタ
  PrintFaultCmd(AtpgMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~PrintFaultCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // type オプションの解析用オブジェクト
  TclPoptStr* mPoptType;

};

END_NAMESPACE_YM_SATPG

#endif // PRINTSTATSCMD_H
