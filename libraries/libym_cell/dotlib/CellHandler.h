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
/// @brief cell group 用のハンドラ
//////////////////////////////////////////////////////////////////////
class CellHandler :
  public Str1GroupHandler
{
public:

  /// @brief コンストラクタ
  /// @param[in] parent 親のハンドラ
  CellHandler(GroupHandler* parent);

  /// @brief デストラクタ
  virtual
  ~CellHandler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部から用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief attribute を設定する．
  /// @param[in] attr_name 属性名
  /// @param[in] value 値
  /// @return 設定が失敗したら false を返す．
  virtual
  bool
  add_attr(const ShString& attr_name,
	   PtValue* value);

  /// @brief leakage_power を追加する．
  virtual
  bool
  add_leakage_power(PtLeakagePower* lp);

  /// @brief ピンを追加する．
  virtual
  bool
  add_pin(PtPin* pin);

  /// @brief バスを追加する．
  virtual
  bool
  add_bus(PtBus* bus);

  /// @brief バンドルを追加する．
  virtual
  bool
  add_bundle(PtBundle* bundle);


protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる GroupHandler の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief group statement の最初に呼ばれる関数
  /// @param[in] attr_name 属性名
  /// @param[in] attr_loc ファイル上の位置
  /// @param[in] value 値
  virtual
  bool
  begin_group(const ShString& attr_name,
	      const FileRegion& attr_loc,
	      const ShString& value);

  /// @brief group statement の最後に呼ばれる関数
  virtual
  bool
  end_group();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 対応する PtCell;
  PtCell* mCell;

};

END_NAMESPACE_YM_CELL_DOTLIB

#endif // LIBYM_CELL_DOTLIB_CELLHANDLER_H
