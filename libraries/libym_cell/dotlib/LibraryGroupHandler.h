#ifndef LIBYM_CELL_DOTLIB_LIBRARYGROUPHANDLER_H
#define LIBYM_CELL_DOTLIB_LIBRARYGROUPHANDLER_H

/// @file libym_cell/dotlib/LibraryGroupHandler.h
/// @brief LibraryGroupHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"
#include "PtNode.h"


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
  // 内部で用いられる仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_token 属性名を表すトークン
  /// @param[in] value_list 値を表すトークンのリスト
  virtual
  bool
  begin_group(Token attr_token,
	      const vector<Token>& value_list);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBRARYGROUPHANDLER_H
