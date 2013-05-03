
/// @file PyLibrary.cc
/// @brief PyLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyLibrary.h"
#include "ym_cell/CellLibrary.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyLibrary;
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyLibrary::PyLibrary(const CellLibrary* library)
{
  mLibrary = library;

  mName = PyObject_FromString(library->name());
  switch ( library->technology() ) {
  case CellLibrary::kTechCmos: mTechnology = PyObject_FromString("cmos"); break;
  case CellLibrary::kTechFpga: mTechnology = PyObject_FromString("fpga"); break;
  }
  const char* dm_str = NULL;
  switch ( library->delay_model() ) {
  case kCellDelayGenericCmos:   dm_str = "generic_cmos"; break;
  case kCellDelayTableLookup:   dm_str = "table_lookup"; break;
  case kCellDelayPiecewiseCmos: dm_str = "piecewise_cmos"; break;
  case kCellDelayCmos2:         dm_str = "cmos2"; break;
  case kCellDelayDcm:           dm_str = "dcm"; break;
  }
  mDelayModel = PyObject_FromString(dm_str);
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
  mCellList = new PyObject*[nc];
  for (ymuint i = 0; i < nc; ++ i) {
    const Cell* cell = library->cell(i);
    PyObject* cell_obj = PyCellCell_FromCell(cell);
    mCellList[i] = cell_obj;
  }

  ymuint ng = library->group_num();
  mGroupList = new PyObject*[ng];
  for (ymuint i = 0; i < ng; ++ i) {
    const CellGroup* group = library->group(i);
    PyObject* group_obj = PyCellGroup_FromCellGroup(group);
    mGroupList[i] = group_obj;
  }

  ymuint nn = library->npn_class_num();
  mClassList = new PyObject*[nn];
  for (ymuint i = 0; i < nn; ++ i) {
    const CellClass* cell_class = library->npn_class(i);
    PyObject* class_obj = PyCellClass_FromCellClass(cell_class);
    mClassList[i] = class_obj;
  }
}

// @brief デストラクタ
PyLibrary::~PyLibrary()
{
  Py_DECREF(mName);
  Py_DECREF(mTechnology);
  Py_DECREF(mDelayModel);
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

  ymuint n = mLibrary->cell_num();
  for (ymuint i = 0; i < n; ++ i) {
    Py_DECREF(mCellList[i]);
  }
  delete [] mCellList;

  ymuint ng = mLibrary->group_num();
  for (ymuint i = 0; i < ng; ++ i) {
    Py_DECREF(mGroupList[i]);
  }
  delete [] mGroupList;

  ymuint nn = mLibrary->npn_class_num();
  for (ymuint i = 0; i < nn; ++ i) {
    Py_DECREF(mClassList[i]);
  }
  delete [] mClassList;
}

// @brief セルを返す．
PyObject*
PyLibrary::cell(ymuint pos)
{
  assert_cond( pos < mLibrary->cell_num(), __FILE__, __LINE__);
  return mCellList[pos];
}

// @brief セルグループを返す．
PyObject*
PyLibrary::cell_group(ymuint pos)
{
  assert_cond( pos < mLibrary->group_num(), __FILE__, __LINE__);
  return mGroupList[pos];
}

// @brief NPN同値クラスを返す．
PyObject*
PyLibrary::npn_class(ymuint pos)
{
  assert_cond( pos < mLibrary->npn_class_num(), __FILE__, __LINE__);
  return mClassList[pos];
}

#if 0
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
#endif

END_NAMESPACE_YM
