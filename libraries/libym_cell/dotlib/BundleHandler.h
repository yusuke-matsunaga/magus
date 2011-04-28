#ifndef LIBYM_CELL_DOTLIB_BUNDLEHANDLER_H
#define LIBYM_CELL_DOTLIB_BUNDLEHANDLER_H

/// @file libym_cell/dotlib/BundleHandler.h
/// @brief BundleHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class BundleHandler BundleHandler.h "BundleHandler.h"
/// @brief bundle group 用のハンドラ
//////////////////////////////////////////////////////////////////////
class BundleHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  BundleHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~BundleHandler();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_BUNDLEHANDLER_H
