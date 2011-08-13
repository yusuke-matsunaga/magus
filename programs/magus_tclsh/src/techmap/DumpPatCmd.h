#ifndef TECHMAP_DUMPPATCMD_H
#define TECHMAP_DUMPPATCMD_H

/// @file DumpPatCmd.h
/// @brief DumpPatCmd のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "TechmapCmd.h"
#include "ym_tclpp/TclPopt.h"


BEGIN_NAMESPACE_MAGUS_TECHMAP

//////////////////////////////////////////////////////////////////////
/// @class DumpPatCmd DumpPatCmd.h "DumpPatCmd.h"
/// @brief パタンのバイナリファイルを出力するコマンド
//////////////////////////////////////////////////////////////////////
class DumpPatCmd :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  DumpPatCmd(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~DumpPatCmd();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // -mislib オプション
  TclPoptStr* mMislibOpt;

  // -dotlib オプション
  TclPoptStr* mDotlibOpt;

};

END_NAMESPACE_MAGUS_TECHMAP

#endif // TECHMAP_DUMPPATCMD_H
