#ifndef LIBYM_CELL_DOTLIB_CELLHANDLER_H
#define LIBYM_CELL_DOTLIB_CELLHANDLER_H

/// @file libym_cell/dotlib/CellHandler.h
/// @brief CellHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "GroupHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class CellHandler CellHandler.h "CellHandler.h"
/// @brief group attribute 用のハンドラ
//////////////////////////////////////////////////////////////////////
class CellHandler :
  public GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parser パーサー
  /// @param[in] parent 親のハンドラ
  CellHandler(DotlibParser& parser,
	      GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~CellHandler();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応する CiCell
  CiCell* mCell;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_CELLHANDLER_H
