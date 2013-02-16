
/// @file PyCell.cc
/// @brief PyCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012 Yusuke Matsunaga
/// All rights reserved.


#include "PyCell.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyCell::PyCell()
{
}

// @brief デストラクタ
PyCell::~PyCell()
{
}

// @brief CellPin のポインタから CellPinObject を得る．
PyObject*
PyCell::get_CellPin(const CellPin* pin)
{
  return get_obj(reinterpret_cast<ympuint>(pin));
}

// @brief ympuint から PyObject* を返す．
// @note なければ NULL となる．
PyObject*
PyCell::get_obj(ympuint ptr)
{
  hash_map<ympuint, PyObject*>::iterator p = mObjMap.find(ptr);
  assert_cond( p != mObjMap.end(), __FILE__, __LINE__);
  return p->second;
}

END_NAMESPACE_YM
