#ifndef YM_CELL_CELLCLASS_H
#define YM_CELL_CELLCLASS_H

/// @file libym_cell/CellClass.h
/// @brief CellClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellClass CellClass.h "ym_cell/CellClass.h"
/// @brief NPN同値なセルグループの集合を表すクラス
//////////////////////////////////////////////////////////////////////
class CellClass
{
public:

  /// @brief コンストラクタ
  CellClass() { }

  /// @brief デストラクタ
  virtual
  ~CellClass() { }


public:
  //////////////////////////////////////////////////////////////////////
  // このクラスに属しているセルグループの情報を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief CellGroup の数を返す．
  virtual
  ymuint
  group_num() const = 0;

  /// @brief CellGroup を返す．
  /// @param[in] pos 位置番号 ( 0 <= pos < func_num() )
  virtual
  const CellGroup*
  cell_group(ymuint pos) const = 0;

};

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLCLASS_H
