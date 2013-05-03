
/// @file PyCellClass.cc
/// @brief PyCellClass の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyCellClass.h"
#include "ym_cell/CellClass.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyCellClass
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyCellClass::PyCellClass(const CellClass* cell_class) :
  mClass(cell_class)
{
  mId = PyObject_FromYmuint32(cell_class->id());
}

// @brief デストラクタ
PyCellClass::~PyCellClass()
{
  Py_DECREF(mId);
}

END_NAMESPACE_YM
