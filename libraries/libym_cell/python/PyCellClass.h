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

class PyLibrary;

//////////////////////////////////////////////////////////////////////
/// @class PyCellClass PyCellClass.h "PyCellClass.h"
/// @brief CellClassObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyCellClass
{
public:

  /// @brief コンストラクタ
  PyCellClass(const CellClass* cell_class,
	      PyLibrary* py_library);

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

  /// @brief 同位体変換を得る．
  PyObject*
  idmap(ymuint pos);

  /// @brief セルグループを得る．
  PyObject*
  cell_group(ymuint pos);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの CellClass
  const CellClass* mClass;

  // 同位体変換のリスト
  PyObject** mIdMapList;

  // セルグループのリスト
  PyObject** mGroupList;

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

END_NAMESPACE_YM

#endif // PYCELLCLASS_H
