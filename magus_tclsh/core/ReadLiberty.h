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
  /// @param[in] library 設定対象のライブラリ
  /// @return 読み込みが成功したら true を返す．
  virtual
  bool
  read_library(const string& filename,
	       ClibCellLibrary& library);

};

END_NAMESPACE_MAGUS

#endif // READLIBERTY_H
