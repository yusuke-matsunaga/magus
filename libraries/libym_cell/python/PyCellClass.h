#ifndef PYCELLCLASS_H
#define PYCELLCLASS_H

/// @file PyCellClass.h
/// @brief PyCellClass のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyCellClass PyCellClass.h "PyCellClass.h"
/// @brief CellClassObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyCellClass
{
public:

  /// @brief コンストラクタ
  PyCellClass(const CellClass* cell_class);

  /// @brief デストラクタ
  ~PyCellClass();


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief CellClass を返す．
  const CellClass*
  cell_class();

  /// @brief ID番号を返す．
  PyObject*
  id();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの CellClass
  const CellClass* mClass;

  // ID
  PyObject* mId;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief CellClass を返す．
inline
const CellClass*
PyCellClass::cell_class()
{
  return mClass;
}

// @brief ID番号を返す．
inline
PyObject*
PyCellClass::id()
{
  return mId;
}

END_NAMESPACE_YM

#endif // PYCELLCLASS_H
