#ifndef TECHMAP_LOADPATCMD_H
#define TECHMAP_LOADPATCMD_H

/// @file LoadPatCmd.h
/// @brief LoadPatCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class LoadPatCmd LoadPatCmd "LoadPatCmd.h"
/// @brief パタンのバイナリデータを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class LoadPatCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  LoadPatCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~LoadPatCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_LOADPATCMD_H
