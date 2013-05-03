
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

// @brief セルを返す．
// @param[in] pos 位置番号
PyObject*
PyCellGroup::cell(ymuint pos)
{
  assert_cond( pos < mGroup->cell_num(), __FILE__, __LINE__);
  return mCellList[pos];
}

// @brief FFセルの場合のピン情報を返す．
// @note FFセル以外の場合には NULL を返す．
PyObject*
PyCellGroup::ff_info()
{
  return NULL;
}

// @brief ラッチセルの場合のピン情報を返す．
// @note ラッチセル以外の場合には NULL を返す．
PyObject*
PyCellGroup::latch_info()
{
  return NULL;
}

// @brief データ入力のピン番号を返す．
// @note データ入力を持たない場合には Py_None を返す．
PyObject*
PyCellGroup::data_pos()
{
  return NULL;
}

// @brief クロック入力のタイプとピン番号のタプルを返す．
PyObject*
PyCellGroup::clock_info()
{
  return NULL;
}

// @brief イネーブル入力のタイプとピン番号のタプルを返す．
PyObject*
PyCellGroup::enable_info()
{
  return NULL;
}

// @brief クリア入力のタイプとピン番号のタプルを返す．
PyObject*
PyCellGroup::clear_info()
{
  return NULL;
}

// @brief プリセット入力のタイプとピン番号のタプルを返す．
PyObject*
PyCellGroup::preset_info()
{
  return NULL;
}

// @brief 肯定出力のピン番号を返す．
PyObject*
PyCellGroup::q_pos()
{
  return NULL;
}

// @biref 否定出力のピン番号を返す．
PyObject*
PyCellGroup::xq_pos()
{
  return NULL;
}

END_NAMESPACE_YM
