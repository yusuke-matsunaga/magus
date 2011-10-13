#ifndef READMISLIB_H
#define READMISLIB_H

/// @file ReadMislib.h
/// @brief ReadMislib のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ReadMislib ReadMislib.h "ReadMislib.h"
/// @ingroup MagusGroup
/// @brief mislib 形式のセルライブラリの読み込みを行うコマンド
//////////////////////////////////////////////////////////////////////
class ReadMislib :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  ReadMislib(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadMislib();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // READMISLIB_H
