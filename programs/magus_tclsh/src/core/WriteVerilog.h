#ifndef MAGUS_CORE_WRITEVERILOG_H
#define MAGUS_CORE_WRITEVERILOG_H

/// @file src/core/WriteVerilog.h
/// @brief WriteVerilog のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIoCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class WriteVerilog BNetIoCmd.h "BNetIoCmd.h"
/// @brief ファイルに Verilog 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////
class WriteVerilog :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  WriteVerilog(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~WriteVerilog();


protected:

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_CORE_WRITEVERILOG_H
