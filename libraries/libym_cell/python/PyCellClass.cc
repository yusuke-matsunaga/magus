
/// @file PyCellClass.cc
/// @brief PyCellClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyCellClass.h"
#include "PyLibrary.h"
#include "ym_cell/CellClass.h"
#include "ym_cell/CellGroup.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyCellClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyCellClass::PyCellClass(const CellClass* cell_class,
			 PyLibrary* py_library) :
  mClass(cell_class)
{
  mId = PyObject_FromYmuint32(cell_class->id());

  ymuint n1 = cell_class->idmap_num();
  mIdMapList = new PyObject*[n1];
  for (ymuint i = 0; i < n1; ++ i) {
    mIdMapList[i] = NULL;
  }

  ymuint n2 = cell_class->group_num();
  mGroupList = new PyObject*[n2];
  for (ymuint i = 0; i < n2; ++ i) {
    const CellGroup* group = cell_class->cell_group(i);
    PyObject* obj = py_library->cell_group(group->id());
    Py_INCREF(obj);
    mGroupList[i] = obj;
  }
}

// @brief デストラクタ
PyCellClass::~PyCellClass()
{
  Py_DECREF(mId);

  ymuint n1 = mClass->idmap_num();
  for (ymuint i = 0; i < n1; ++ i) {
    Py_DECREF(mIdMapList[i]);
  }
  delete [] mIdMapList;

  ymuint n2 = mClass->group_num();
  for (ymuint i = 0; i < n2; ++ i) {
    Py_DECREF(mGroupList[i]);
  }
  delete [] mGroupList;
}

// @brief 同位体変換を得る．
PyObject*
PyCellClass::idmap(ymuint pos)
{
  assert_cond( pos < mClass->idmap_num(), __FILE__, __LINE__);
  return mIdMapList[pos];
}

// @brief セルグループを得る．
PyObject*
PyCellClass::cell_group(ymuint pos)
{
  assert_cond( pos < mClass->group_num(), __FILE__, __LINE__);
  return mGroupList[pos];
}

END_NAMESPACE_YM
