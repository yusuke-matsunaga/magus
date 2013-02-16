
/// @file PyLibrary.cc
/// @brief PyLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "PyLibrary.h"


BEGIN_NAMESPACE_YM

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
  return get_obj(reinterpret_cast<ympuint>(cell));
}

// @brief CellGroup のポインタから CellGroupObject を得る．
PyObject*
PyLibrary::get_CellGroup(const CellGroup* cell_group)
{
  return get_obj(reinterpret_cast<ympuint>(cell_group));
}

// @brief CellClass のポインタから CellClassObject を得る．
PyObject*
PyLibrary::get_CellClass(const CellClass* cell_class)
{
  return get_obj(reinterpret_cast<ympuint>(cell_class));
}

// @brief CellLutTemplate のポインタから CellLutTemplateObject を得る．
PyObject*
PyLibrary::get_CellLutTemplate(const CellLutTemplate* cell_lut_template)
{
  return get_obj(reinterpret_cast<ympuint>(cell_lut_template));
}

// @brief ympuint から PyObject* を返す．
// @note なければエラーとなる．
PyObject*
PyLibrary::get_obj(ympuint ptr)
{
  hash_map<ympuint, PyObject*>::iterator p = mObjMap.find(ptr);
  assert_cond( p != mObjMap.end(), __FILE__, __LINE__);
  return p->second;
}

END_NAMESPACE_YM
