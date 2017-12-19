#ifndef MAGUS_CORE_WRITEBLIF_H
#define MAGUS_CORE_WRITEBLIF_H

/// @file src/core/WriteBlif.h
/// @brief WriteBlif のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: BNetIoCmd.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "tclsh/NetCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class WriteBlif WriteBlif.h "WriteBlif.h"
/// @brief ファイルに blif 形式で書き出すコマンド
//////////////////////////////////////////////////////////////////////
class WriteBlif :
  public NetCmd
{
public:

  /// @brief コンストラクタ
  WriteBlif(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~WriteBlif();


protected:
  //////////////////////////////////////////////////////////////////////
  // TclCmd の仮想関数のオーバーロード
  //////////////////////////////////////////////////////////////////////

  /// @brief コマンド処理関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // MAGUS_CORE_WRITEBLIF_H
