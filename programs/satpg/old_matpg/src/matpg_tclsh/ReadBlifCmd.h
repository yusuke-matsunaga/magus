#ifndef SRC_MATPG_READBLIFCMD_H
#define SRC_MATPG_READBLIFCMD_H

/// @file src/matpg/ReadBlifCmd.h
/// @brief ReadBlifCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ReadBlifCmd.h 2043 2009-02-21 07:08:48Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#include "matpg_nsdef.h"
#include "ym_YmTclpp/TclCmd.h"
#include "ym_YmNetworks/TgNetwork.h"
#include "ym_YmNetworks/TgBlifReader.h"


BEGIN_NAMESPACE_YM_MATPG

//////////////////////////////////////////////////////////////////////
// 故障の設定を行うコマンド
//////////////////////////////////////////////////////////////////////
class ReadBlifCmd :
  public TclCmd
{
public:

  /// @brief コンストラクタ
  ReadBlifCmd();

  /// @brief デストラクタ
  virtual
  ~ReadBlifCmd();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // blif パーサ
  TgBlifReader mReader;

  // 読み込んだネットワークを保存しておくクラス
  TgNetwork mNetwork;

};

END_NAMESPACE_YM_MATPG

#endif // SRC_MATPG_READBLIFCMD_H
