
/// @file PyCellGroup.cc
/// @brief PyCellGroup の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyCellGroup.h"
#include "ym_cell/CellGroup.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyCellGroup
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyCellGroup::PyCellGroup(const CellGroup* group) :
  mGroup(group)
{
  mId = PyObject_FromYmuint32(group->id());
}

// @brief デストラクタ
PyCellGroup::~PyCellGroup()
{
  Py_DECREF(mId);
}

END_NAMESPACE_YM
