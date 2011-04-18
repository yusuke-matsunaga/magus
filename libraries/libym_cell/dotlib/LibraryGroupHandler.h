#ifndef LIBYM_CELL_DOTLIB_LIBRARYGROUPHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBRARYGROUPHANDLER_H

/// @file libym_cell/dotlib/LibraryGroupHandler.h
/// @brief LibraryGroupHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LibraryGroupHandler LibraryGroupHandler.h "LibraryGroupHandler.h"
/// @brie library グループ属性
//////////////////////////////////////////////////////////////////////
class LibraryGroupHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  LibraryGroupHandler(DotLibParser& parser);

  /// @brief デストラクタ
  virtual
  ~LibraryGroupHandler();


private:
  //////////////////////////////////////////////////////////////////////
  // GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief グループ名を読み込んだ時の処理
  /// @param[in] name 属性名
  /// @param[in] group_name グループ名
  virtual
  bool
  read_group_name(const char* name,
		  const string& group_name);


};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBRARYGROUPHANDLER_H
