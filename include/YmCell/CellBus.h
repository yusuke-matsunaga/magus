#ifndef CELL_CELLBUS_H
#define CELL_CELLBUS_H

/// @file YmCell/CellBus.h
/// @brief CellBus のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "YmCell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellBus CellBus.h "YmCell/CellBus.h"
/// @brief バスを表すクラス
//////////////////////////////////////////////////////////////////////
class CellBus
{
public:

  /// @brief コンストラクタ
  CellBus() { }

  /// @brief デストラクタ
  virtual
  ~CellBus() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief バスの型の取得
  virtual
  const CellBusType*
  bus_type() const = 0;

  /// @brief ピン数の取得
  virtual
  ymuint
  pin_num() const = 0;

  /// @brief ピンの取得
  /// @param[in] pos 位置番号 ( 0 <= pos < pin_num() )
  virtual
  const CellPin*
  pin(ymuint pos) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLBUS_H
