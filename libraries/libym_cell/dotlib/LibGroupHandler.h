#ifndef LIBYM_CELL_DOTLIB_LIBGROUPHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBGROUPHANDLER_H

/// @file libym_cell/dotlib/LibGroupHandler.h
/// @brief LibGroupHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

class LibrarhGroupHandler;

//////////////////////////////////////////////////////////////////////
/// @class LibGroupHandler LibGroupHandler.h "LibGroupHandler.h"
/// @brief library グループの group statement　用ハンドラ
//////////////////////////////////////////////////////////////////////
class LibGroupHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] lib_handler 親のハンドラ
  LibGroupHandler(LibraryGroupHandler* lib_handler);

  /// @brief デストラクタ
  virtual
  ~LibGroupHandler();


protected:
  //////////////////////////////////////////////////////////////////////
  // 継承クラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のハンドラを返す．
  LibraryGroupHandler*
  lib_handler();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親のハンドラ
  LibraryGroupHandler* mLibHandler;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBGROUPHANDLER_H
