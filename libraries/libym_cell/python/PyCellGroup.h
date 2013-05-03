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

//////////////////////////////////////////////////////////////////////
/// @class PyCellGroup PyCellGroup.h "PyCellGroup.h"
/// @brief CellGroupObject のための補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyCellGroup
{
public:

  /// @brief コンストラクタ
  PyCellGroup(const CellGroup* group);

  /// @brief デストラクタ
  virtual
  ~PyCellGroup();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // もとの CellGroup
  const CellGroup* mGroup;

  // ID
  PyObject* mId;

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

END_NAMESPACE_YM

#endif // PYCELLGROUP_H
