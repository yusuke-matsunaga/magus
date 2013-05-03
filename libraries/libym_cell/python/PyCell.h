#ifndef PYCELL_H
#define PYCELL_H

/// @file PyCell.h
/// @brief PyCell のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

class PyPin;

//////////////////////////////////////////////////////////////////////
/// @class PyCell PyCell.h "Pycell.h"
/// @brief CellObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyCell
{
public:

  /// @brief コンストラクタ
  PyCell(const Cell* cell);

  /// @brief デストラクタ
  ~PyCell();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief Cell を得る．
  const Cell*
  cell();

  /// @brief ID番号を得る．
  PyObject*
  id();

  /// @brief 名前を得る．
  PyObject*
  name();

  /// @brief 面積を得る．
  PyObject*
  area();

  /// @brief ピン番号からピンを得る．
  PyObject*
  pin(ymuint pin_id);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // Cell の本体
  const Cell* mCell;

  // ID 番号を表すオブジェクト
  PyObject* mId;

  // 名前を表すオブジェクト
  PyObject* mName;

  // area を表すオブジェクト
  PyObject* mArea;

  // ピンを表すオブジェクトの配列
  PyObject** mPinArray;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief Cell を得る．
inline
const Cell*
PyCell::cell()
{
  return mCell;
}

// @brief ID番号を得る．
inline
PyObject*
PyCell::id()
{
  return mId;
}

// @brief 名前を得る．
inline
PyObject*
PyCell::name()
{
  return mName;
}

// @brief 面積を得る．
inline
PyObject*
PyCell::area()
{
  return mArea;
}

END_NAMESPACE_YM

#endif // PYCELL_H
