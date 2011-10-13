#ifndef READLIBERTY_H
#define READLIBERTY_H

/// @file ReadLiberty.h
/// @brief ReadLiberty のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "MagCmd.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ReadLiberty ReadLiberty.h "ReadLiberty.h"
/// @ingroup MagusGroup
/// @brief liberty 形式のセルライブラリの読み込みを行うコマンド
//////////////////////////////////////////////////////////////////////
class ReadLiberty :
  public MagCmd
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  ReadLiberty(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadLiberty();


protected:

  /// @brief コマンドを実行する仮想関数
  virtual
  int
  cmd_proc(TclObjVector& objv);

};

END_NAMESPACE_MAGUS

#endif // READLIBERTY_H
