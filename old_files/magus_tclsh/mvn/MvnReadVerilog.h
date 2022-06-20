#ifndef MVNREADVERILOG_H
#define MVNREADVERILOG_H

/// @file MvnReadVerilog.h
/// @brief MvnReadVerilog のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2017 Yusuke Matsunaga
/// All rights reserved.


#include "MvnCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class MvnReadVerilog MvnReadVerilog "MvnReadVerilog"
/// @brief verilog ファイルを読み込むコマンド
//////////////////////////////////////////////////////////////////////
class MvnReadVerilog :
  public MvnCmd
{
public:

  /// @brief コンストラクタ
  MvnReadVerilog(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~MvnReadVerilog();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MVNREADVERILOG_H
