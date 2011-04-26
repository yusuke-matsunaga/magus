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
  /// @param[in] parent 親のハンドラ
  CellHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~CellHandler();


#if 0
public:
  //////////////////////////////////////////////////////////////////////
  // 他のクラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する PtNode を返す．
  virtual
  PtNode*
  pt_node();
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる仮想関数
  //////////////////////////////////////////////////////////////////////

#if 0
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
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応する PtCell;
  PtCell* mCell;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_CELLHANDLER_H
