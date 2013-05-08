
/// @file PyLut.cc
/// @brief PyLut の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyLut.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyLut
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyLut::PyLut(const CellLut* lut) :
  mLut(lut)
{
}

// @brief デストラクタ
PyLut::~PyLut()
{
  Py_DECREF(mLutTemplate);
}

// @brief テンプレート名を返す．
PyObject*
PyLut::template_name()
{
  return PyObject_FromString(lut()->template_name());
}

// @brief 変数型の取得
PyObject*
PyLut::variable_type(ymuint var)
{
  assert_cond( var < lut()->dimension(), __FILE__, __LINE__);

  tCellVarType var_type = lut()->variable_type(var);
  string vt_str;
  switch ( var_type ) {
  case kVarInputNetTransition:
    vt_str = "input_net_transition";
    break;
  case kVarTotalOutputNetCapacitance:
    vt_str = "total_output_net_capacitance";
    break;
  case kVarOutputNetLength:
    vt_str = "output_net_length";
    break;
  case kVarOutputNetWireCap:
    vt_str = "output_net_wire_cap";
    break;
  case kVarOutputNetPinCap:
    vt_str = "output_net_pin_cap";
    break;
  case kVarRelatedOutTotalOutputNetCapacitance:
    vt_str = "related_out_total_output_net_capacitance";
    break;
  case kVarRelatedOutOutputNetLength:
    vt_str = "related_out_output_net_length";
    break;
  case kVarRelatedOutOutputNetWireCap:
    vt_str = "related_out_output_net_wire_cap";
    break;
  case kVarRelatedOutOutputNetPinCap:
    vt_str = "related_out_output_net_pin_cap";
    break;
  case kVarConstrainedPinTransition:
    vt_str = "constrained_pin_transition";
    break;
  case kVarRelatedPinTransition:
    vt_str = "related_pin_transition";
    break;
  case kVarNone:
    vt_str = "none";
    break;
  }
  return PyObject_FromString(vt_str);
}

// @brief インデックス値の取得
PyObject*
PyLut::index(ymuint var,
	     ymuint pos)
{
  assert_cond( var < lut()->dimension(), __FILE__, __LINE__);
  assert_cond( pos < lut()->index_num(var), __FILE__, __LINE__);
  return PyObject_FromDouble(lut()->index(var, pos));
}

// @brief 格子点の値の取得
PyObject*
PyLut::grid_value(const vector<ymuint>& pos_array)
{
  assert_cond( pos_array.size() == lut()->dimension(), __FILE__, __LINE__);
  return PyObject_FromDouble(lut()->grid_value(pos_array));
}

// @brief 値の取得
PyObject*
PyLut::value(const vector<double>& val_array)
{
  assert_cond( val_array.size() == lut()->dimension(), __FILE__, __LINE__);
  return PyObject_FromDouble(lut()->value(val_array));
}

END_NAMESPACE_YM
