#ifndef PYLUT_H
#define PYLUT_H

/// @file PyLut.h
/// @brief PyLut のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/pycell.h"
#include "ym_cell/CellLut.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
/// @class PyLut PyLut.h "PyLut.h"
/// @brief CellLut を表す補助的なクラス
//////////////////////////////////////////////////////////////////////
class PyLut
{
public:

  /// @brief コンストラクタ
  PyLut(const CellLut* lut);

  /// @brief デストラクタ
  ~PyLut();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief CellLut を返す．
  const CellLut*
  lut();

  /// @brief CellLutTemplate を返す．
  PyObject*
  lut_template();

  /// @brief テンプレート名を返す．
  PyObject*
  template_name();

  /// @brief 変数型の取得
  PyObject*
  variable_type(ymuint var);

  /// @brief インデックス値の取得
  PyObject*
  index(ymuint var,
	ymuint pos);

  /// @brief 格子点の値の取得
  PyObject*
  grid_value(const vector<ymuint>& pos_array);

  /// @brief 値の取得
  PyObject*
  value(const vector<double>& val_array);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // CellLut
  const CellLut* mLut;

  // テンプレート
  PyObject* mLutTemplate;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief CellLut を返す．
inline
const CellLut*
PyLut::lut()
{
  return mLut;
}

// @brief CellLutTemplate を返す．
inline
PyObject*
PyLut::lut_template()
{
  return mLutTemplate;
}

END_NAMESPACE_YM

#endif // PYLUT_H
