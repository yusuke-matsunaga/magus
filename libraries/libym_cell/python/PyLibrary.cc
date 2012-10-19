
/// @file PyLibrary.cc
/// @brief PyLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "PyLibrary.h"


BEGIN_NAMESPACE_YM_PYTHON

//////////////////////////////////////////////////////////////////////
// クラス PyLibrary;
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyLibrary::PyLibrary()
{
}

// @brief デストラクタ
PyLibrary::~PyLibrary()
{
}

// @brief Cell のポインタから CellObject を得る．
PyObject*
PyLibrary::get_Cell(const Cell* cell)
{
  return NULL;
}

// @brief CellGroup のポインタから CellGroupObject を得る．
PyObject*
PyLibrary::get_CellGroup(const CellGroup* cell_group)
{
  return NULL;
}

// @brief CellClass のポインタから CellClassObject を得る．
PyObject*
PyLibrary::get_CellClass(const CellClass* cell_class)
{
  return NULL;
}

// @brief CellLutTemplate のポインタから CellLutTemplateObject を得る．
PyObject*
PyLibrary::get_CellLutTemplate(const CellLutTemplate* cell_lut_template)
{
  return NULL;
}

END_NAMESPACE_YM_PYTHON
