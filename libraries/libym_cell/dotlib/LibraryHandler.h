#ifndef LIBYM_CELL_DOTLIB_LIBRARYHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBRARYHANDLER_H

/// @file libym_cell/dotlib/LibraryHandler.h
/// @brief LibraryHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LibraryHandler LibraryHandler.h "LibraryHandler.h"
/// @brie library グループ属性
//////////////////////////////////////////////////////////////////////
class LibraryHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] ptmgr パース木を管理するオブジェクト
  LibraryHandler(DotlibParser& parser,
		 PtMgr& ptmgr);

  /// @brief デストラクタ
  virtual
  ~LibraryHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のハンドラを得る．
  /// @note このクラスは NULL を返す．
  virtual
  GroupHandler*
  parent();


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBRARYHANDLER_H
