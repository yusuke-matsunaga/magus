#ifndef MAGUS_LOGBASE_BNETIOCMD_H
#define MAGUS_LOGBASE_BNETIOCMD_H

/// @file magus/logbase/BNetIoCmd.h
/// @brief ネットワーク入出力コマンドのヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIoCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
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

#endif // MAGUS_LOGBASE_BNETIOCMD_H
