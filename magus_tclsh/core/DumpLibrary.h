#ifndef DUMPLIBRARY_H
#define DUMPLIBRARY_H

/// @file DumpLibrary.h
/// @brief DumpLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "tclsh/MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class DumpLibrary DumpLibrary.h "DumpLibrary.h"
/// @brief セルライブラリをバイナリファイルに出力するコマンド
//////////////////////////////////////////////////////////////////////
class DumpLibrary :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  DumpLibrary(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DumpLibrary();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // DUMPLIBRARY_H
