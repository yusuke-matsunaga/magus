#ifndef CELL_CELLBUSTYPE_H
#define CELL_CELLBUSTYPE_H

/// @file YmCell/CellBusType.h
/// @brief CellBusType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.

#include "YmCell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellBusType CellBus.h "YmCell/CellBus.h"
/// @brief バスタイプを表すクラス
//////////////////////////////////////////////////////////////////////
class CellBusType
{
public:

  /// @brief base_type を表す型
  enum tBaseType {
    kArrayType
  };

  /// @brief data_type を表す型
  enum tDataType {
    kBitType
  };


public:

  /// @brief コンストラクタ
  CellBusType() { }

  /// @brief デストラクタ
  virtual
  ~CellBusType() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 属性の取得
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前の取得
  virtual
  string
  name() const = 0;

  /// @brief base_type の取得
  virtual
  tBaseType
  base_type() const = 0;

  /// @brief data_type の取得
  virtual
  tDataType
  data_type() const = 0;

  /// @brief ビット幅の取得
  virtual
  ymuint
  bit_width() const = 0;

  /// @brief 開始ビットの取得
  virtual
  ymint
  bit_from() const = 0;

  /// @brief 終了ビットの取得
  virtual
  ymint
  bit_to() const = 0;

  /// @brief 向きの取得
  /// @note true の時，降順を表す．
  virtual
  bool
  downto() const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLBUS_H
