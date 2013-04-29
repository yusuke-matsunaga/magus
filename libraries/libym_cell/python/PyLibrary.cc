
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
PyLibrary::PyLibrary(const CellLibrary* library)
{
  mLibrary = library;

  mBusNamingStyle = PyObject_FromString(library->bus_naming_style());
  mDate = PyObject_FromString(library->date());
  mRevision = PyObject_FromString(library->revision());
  mComment = PyObject_FromString(library->comment());
  mTimeUnit = PyObject_FromString(library->time_unit());
  mVoltageUnit = PyObject_FromString(library->voltage_unit());
  mCurrentUnit = PyObject_FromString(library->current_unit());
  mPullingResistanceUnit = PyObject_FromString(library->pulling_resistance_unit());
  double unit_val = library->capacitive_load_unit();
  string unit_str = library->capacitive_load_unit_str();
  mCapacitiveLoadUnit = Py_BuildValue("(ds)", unit_val, unit_str.c_str());
  mLeakagePowerUnit = PyObject_FromString(library->leakage_power_unit());

  ymuint nc = library->cell_num();
  mCellList = PyList_New(nc);
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
    PyObject* cell_obj = new Py_Cell(cell);
    mObjMap.insert(make_pair(cell, cell_obj));
    PyList_SetItem(mCellList, i, cell_obj);
  }
}

// @brief デストラクタ
PyLibrary::~PyLibrary()
{
  Py_DECREF(mBusNamingStyle);
  Py_DECREF(mDate);
  Py_DECREF(mRevision);
  Py_DECREF(mComment);
  Py_DECREF(mTimeUnit);
  Py_DECREF(mVoltageUnit);
  Py_DECREF(mCurrentUnit);
  Py_DECREF(mPullingResistanceUnit);
  Py_DECREF(mCapacitiveLoadUnit);
  Py_DECREF(mLeakagePowerUnit);
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
  PyObject* result = p->second;

  Py_INCREF(result);
  return result;
}

END_NAMESPACE_YM
