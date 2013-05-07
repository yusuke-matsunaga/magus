
/// @file PyLutTemplate.cc
/// @brief PyLutTemplate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2013 Yusuke Matsunaga
/// All rights reserved.


#include "PyLutTemplate.h"
#include "ym_cell/CellLut.h"


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// クラス PyLutTemplate
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PyLutTemplate::PyLutTemplate(const CellLutTemplate* lut_template) :
  mLutTemplate(lut_template)
{
}

// @brief デストラクタ
PyLutTemplate::~PyLutTemplate()
{
}

// @brief 名前を返す．
PyObject*
PyLutTemplate::name()
{
  return PyObject_FromString(lut_template()->name());
}

// @brief 変数型を返す．
// @param[in] var 変数番号
PyObject*
PyLutTemplate::variable_type(ymuint var)
{
  ymuint dim = lut_template()->dimension();
  assert_cond( var < dim, __FILE__, __LINE__);

  tCellVarType var_type = lut_template()->variable_type(var);
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

// @brief デフォルトインデックス値を返す．
PyObject*
PyLutTemplate::index(ymuint var,
		     ymuint pos)
{
  ymuint dim = lut_template()->dimension();
  assert_cond( var < dim, __FILE__, __LINE__);

  ymuint n = lut_template()->index_num(var);
  assert_cond( pos < n, __FILE__, __LINE__);

  double index = lut_template()->index(var, pos);
  return PyObject_FromDouble(index);
}

END_NAMESPACE_YM
