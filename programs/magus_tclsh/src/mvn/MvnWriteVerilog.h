#ifndef SRC_LOGBASE_MVNWRITEVERILOG_H
#define SRC_LOGBASE_MVNWRITEVERILOG_H

/// @file src/logbase/MvnWriteVerilog.h
/// @brief MvnWriteVerilog のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: Lutmap.h 2274 2009-06-10 07:45:29Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class MvnWriteVerilog MvnWriteVerilog "MvnWriteVerilog"
/// @brief verilog ファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class MvnWriteVerilog :
  public MvnCmd
{
public:

  /// @brief コンストラクタ
  MvnWriteVerilog(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~MvnWriteVerilog();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // SRC_LOGBASE_MVNWRITEVERILOG_H
