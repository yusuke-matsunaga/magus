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
  /// @param[in] ptmgr パース木を管理するオブジェクト
  LibraryHandler(DotlibParser& parser,
		 PtMgr& ptmgr);

  /// @brief デストラクタ
  virtual
  ~LibraryHandler();


public:

  /// @brief パース木の根のノードを返す．
  const PtLibrary*
  library();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のハンドラを得る．
  /// @note このクラスは NULL を返す．
  virtual
  GroupHandler*
  parent();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief simple attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_simple_attr(const ShString& attr_name,
		  const PtValue* value);

  /// @brief complex attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value_list 値のリスト
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_complex_attr(const ShString& attr_name,
		   const vector<const PtValue*>& value_list);

  /// @brief セルを追加する．
  virtual
  bool
  add_cell(PtCell* cell);


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value_list 値を表すトークンのリスト
  virtual
  bool
  begin_group(const ShString& attr_name,
	      const FileRegion& attr_loc,
	      const vector<const PtValue*>& value_list);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ライブラリを表す PtNode
  PtLibrary* mLibrary;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_LIBRARYHANDLER_H
