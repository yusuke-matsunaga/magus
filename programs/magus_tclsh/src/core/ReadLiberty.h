#ifndef READLIBERTY_H
#define READLIBERTY_H

/// @file ReadLiberty.h
/// @brief ReadLiberty のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadLibrary.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ReadLiberty ReadLiberty.h "ReadLiberty.h"
/// @ingroup MagusGroup
/// @brief liberty 形式のセルライブラリの読み込みを行うコマンド
//////////////////////////////////////////////////////////////////////
class ReadLiberty :
  public ReadLibrary
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  ReadLiberty(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadLiberty();


private:
  //////////////////////////////////////////////////////////////////////
  // ReadLibrary の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief セルライブラリを読み込む．
  /// @param[in] filename ファイル名
  virtual
  const CellLibrary*
  read_library(const string& filename);

};

END_NAMESPACE_MAGUS

#endif // READLIBERTY_H
