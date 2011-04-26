#ifndef LIBYM_CELL_DOTLIB_BUSHANDLER_H
#define LIBYM_CELL_DOTLIB_BUSHANDLER_H

/// @file libym_cell/dotlib/BusHandler.h
/// @brief BusHandler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PinHolderHandler.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

class CellHandler;

//////////////////////////////////////////////////////////////////////
/// @class BusHandler BusHandler.h "BusHandler.h"
/// @brief bus group 用のハンドラ
//////////////////////////////////////////////////////////////////////
class BusHandler :
  public PinHolderHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  BusHandler(CellHandler* parent);

  /// @brief デストラクタ
  virtual
  ~BusHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // DotlibHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 親のハンドラを得る．
  virtual
  GroupHandler*
  parent();


public:
  //////////////////////////////////////////////////////////////////////
  // 他のクラスから用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 対応する PtNode を返す．
  virtual
  PtNode*
  pt_node();

  /// @brief ピンを追加する．
  virtual
  void
  add_pin(PtPin* pin);


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

  // 親のハンドラ
  CellHandler* mParent;

  // 対応する PtBus
  PtBus* mBus;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_BUSHANDLER_H
