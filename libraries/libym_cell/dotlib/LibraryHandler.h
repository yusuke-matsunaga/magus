#ifndef LIBYM_CELL_DOTLIB_LIBRARYHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBRARYHANDLER_H

/// @file libym_cell/dotlib/LibraryHandler.h
/// @brief LibraryHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"
#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class LibraryHandler LibraryHandler.h "LibraryHandler.h"
/// @brie library グループ属性
//////////////////////////////////////////////////////////////////////
class LibraryHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  LibraryHandler(DotlibParser& parser);

  /// @brief デストラクタ
  virtual
  ~LibraryHandler();


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値を表すトークンのリスト
  virtual
  bool
  begin_group(const string& attr_name,
	      const FileRegion& attr_loc,
	      const vector<Token>& value_list);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBRARYHANDLER_H
