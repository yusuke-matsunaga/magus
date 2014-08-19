#ifndef READMISLIB_H
#define READMISLIB_H

/// @file ReadMislib.h
/// @brief ReadMislib のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadLibrary.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class ReadMislib ReadMislib.h "ReadMislib.h"
/// @ingroup MagusGroup
/// @brief mislib 形式のセルライブラリの読み込みを行うコマンド
//////////////////////////////////////////////////////////////////////
class ReadMislib :
  public ReadLibrary
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  ReadMislib(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~ReadMislib();


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

#endif // READMISLIB_H
