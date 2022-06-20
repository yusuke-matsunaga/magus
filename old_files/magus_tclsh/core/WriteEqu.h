#ifndef WRITE_EQU_H
#define WRITE_EQU_H

/// @file WriteEqu.h
/// @brief WriteEqu のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "BNetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class WriteEqu BNetIoCmd.h "BNetIoCmd.h"
/// @brief ファイルに EQU 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////
class WriteEqu :
  public BNetCmd
{
public:

  /// @brief コンストラクタ
  WriteEqu(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~WriteEqu();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // WRITE_EQU_H
