#ifndef RESTORELIBRARY_H
#define RESTORELIBRARY_H

/// @file RestoreLibrary.h
/// @brief RestoreLibrary のヘッダファイル
///
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ReadLibrary.h"


BEGIN_NAMESPACE_MAGUS

//////////////////////////////////////////////////////////////////////
/// @class RestoreLibrary RestoreLibrary.h "RestoreLibrary.h"
/// @ingroup MagusGroup
/// @brief バイナリ形式のファイルを読み込んでセルライブラリを設定するコマンド
//////////////////////////////////////////////////////////////////////
class RestoreLibrary :
  public ReadLibrary
{
public:

  /// @brief コンストラクタ
  /// @param[in] mgr Magus の管理オブジェクト
  RestoreLibrary(MagMgr* mgr);

  /// @brief デストラクタ
  virtual
  ~RestoreLibrary();


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

#endif // RESTORELIBRARY_H
