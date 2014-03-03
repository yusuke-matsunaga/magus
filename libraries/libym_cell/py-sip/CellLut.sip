
/// @file CellLut.sip
/// @brief CellLut の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellLut /Abstract/
{
%TypeHeaderCode
#include "cell/CellLut.h"
using nsYm::CellLut;
%End

public:

  /// @brief テンプレートの取得
  const CellLutTemplate*
  lut_template() const;

  /// @brief テンプレート名の取得
  const char*
  template_name() const;

  /// @brief 次元数の取得
  unsigned int
  dimension() const;

  /// @brief 変数型の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  tCellVarType
  variable_type(unsigned int var) const;

  /// @brief インデックス数の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  unsigned int
  index_num(unsigned int var) const;

  /// @brief インデックス値の取得
  /// @param[in] var 変数番号 ( 0 <= var < dimension() )
  /// @param[in] pos 位置番号 ( 0 <= pos < index_num(var) )
  double
  index(unsigned int var,
	unsigned int pos) const;

  /// @brief 格子点の値の取得
  /// @param[in] pos_array 格子点座標
  /// @note pos_array のサイズは dimension() と同じ
  double
  grid_value(SIP_PYLIST) const;
  %MethodCode
  ymuint n = PyList_Size(a0);
  std::vector<ymuint> pos_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    ymuint pos = PyInt_AsLong(obj1);
    pos_array[i] = pos;
  }
  sipRes = sipCpp->grid_value(pos_array);
  %End

  /// @brief 値の取得
  /// @param[in] val_array 入力の値の配列
  /// @note val_array のサイズは dimension() と同じ
  double
  value(SIP_PYLIST) const;
  %MethodCode
  ymuint n = PyList_Size(a0);
  std::vector<double> val_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    PyObject* obj1 = PyList_GetItem(a0, i);
    double val = PyFloat_AsDouble(obj1);
    val_array[i] = val;
  }
  sipRes = sipCpp->value(val_array);
  %End


private:

  ~CellLut();

};
