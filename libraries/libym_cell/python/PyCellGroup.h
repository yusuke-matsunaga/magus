#ifndef PYCELLGROUP_H
#define PYCELLGROUP_H

/// @file PyCellGroup.h
/// @brief PyCellGroup のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"


BEGIN_NAMESPACE_YM

class PyLibrary;

//////////////////////////////////////////////////////////////////////
/// @class PyCellGroup PyCellGroup.h "PyCellGroup.h"
/// @brief CellGroupObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyCellGroup
{
public:

  /// @brief コンストラクタ
  PyCellGroup(const CellGroup* group,
	      PyLibrary* py_library);

  /// @brief デストラクタ
  virtual
  ~PyCellGroup();

  /// @brief 代表クラスを設定する．
  void
  set_rep(PyObject* rep);


public:
  //////////////////////////////////////////////////////////////////////
  // 共通の属性
  //////////////////////////////////////////////////////////////////////

  /// @brief CellGroup を返す．
  const CellGroup*
  cell_group();

  /// @brief ID番号を返す．
  PyObject*
  id();

  /// @brief 代表クラスを返す．
  PyObject*
  rep_class();

  /// @brief 代表クラスに対する変換マップを返す．
  PyObject*
  map();

  /// @brief セルを返す．
  /// @param[in] pos 位置番号
  PyObject*
  cell(ymuint pos);


public:
  //////////////////////////////////////////////////////////////////////
  // FF/ラッチセルの場合にピンの情報を返す関数
  //////////////////////////////////////////////////////////////////////

  /// @brief FFセルの場合のピン情報を返す．
  /// @note FFセル以外の場合には NULL を返す．
  virtual
  PyObject*
  ff_info();

  /// @brief ラッチセルの場合のピン情報を返す．
  /// @note ラッチセル以外の場合には NULL を返す．
  virtual
  PyObject*
  latch_info();

  /// @brief データ入力のピン番号を返す．
  /// @note データ入力を持たない場合には Py_None を返す．
  virtual
  PyObject*
  data_pos();

  /// @brief クロック入力のタイプとピン番号のタプルを返す．
  virtual
  PyObject*
  clock_info();

  /// @brief イネーブル入力のタイプとピン番号のタプルを返す．
  virtual
  PyObject*
  enable_info();

  /// @brief クリア入力のタイプとピン番号のタプルを返す．
  virtual
  PyObject*
  clear_info();

  /// @brief プリセット入力のタイプとピン番号のタプルを返す．
  virtual
  PyObject*
  preset_info();

  /// @brief 肯定出力のピン番号を返す．
  virtual
  PyObject*
  q_pos();

  /// @biref 否定出力のピン番号を返す．
  virtual
  PyObject*
  xq_pos();


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの CellGroup
  const CellGroup* mGroup;

  // ID
  PyObject* mId;

  // 代表クラス
  PyObject* mRepClass;

  // 代表クラスに対する変換マップ
  PyObject* mMap;

  // セルの配列
  PyObject** mCellList;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief CellGroup を返す．
inline
const CellGroup*
PyCellGroup::cell_group()
{
  return mGroup;
}

// @brief ID番号を返す．
inline
PyObject*
PyCellGroup::id()
{
  return mId;
}

// @brief 代表クラスを返す．
inline
PyObject*
PyCellGroup::rep_class()
{
  return mRepClass;
}

// @brief 代表クラスに対する変換マップを返す．
inline
PyObject*
PyCellGroup::map()
{
  return mMap;
}

END_NAMESPACE_YM

#endif // PYCELLGROUP_H
