
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
  mName = PyObject_FromString(lut_template->name());
  ymuint dim = lut_template->dimension();
  mVariableTypeArray = new PyObject*[dim];
  for (ymuint i = 0; i < dim; ++ i) {
    tCellVarType var_type = lut_template->variable_type(i);
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
    mVariableTypeArray[i] = PyObject_FromString(vt_str);
  }

  mIndexArray = new PyObject**[dim];
  for (ymuint var = 0; var < dim; ++ var) {
    ymuint n = lut_template->index_num(var);
    mIndexArray[var] = new PyObject*[n];
    for (ymuint i = 0; i < n; ++ i) {
      double index = lut_template->index(var, i);
      mIndexArray[var][i] = PyObject_FromDouble(index);
    }
  }
}

// @brief デストラクタ
PyLutTemplate::~PyLutTemplate()
{
  Py_DECREF(mName);

  ymuint dim = mLutTemplate->dimension();

  for (ymuint var = 0; var < dim; ++ var) {
    Py_DECREF(mVariableTypeArray[var]);
  }
  delete [] mVariableTypeArray;

  for (ymuint var = 0; var < dim; ++ var) {
    ymuint n = mLutTemplate->index_num(var);
    for (ymuint i = 0; i < n; ++ i) {
      Py_DECREF(mIndexArray[var][i]);
    }
    delete [] mIndexArray[var];
  }
  delete [] mIndexArray;
}

// @brief 変数型を返す．
// @param[in] var 変数番号
PyObject*
PyLutTemplate::variable_type(ymuint var)
{
  ymuint dim = mLutTemplate->dimension();
  assert_cond( var < dim, __FILE__, __LINE__);
  return mVariableTypeArray[var];
}

// @brief デフォルトインデックス値を返す．
PyObject*
PyLutTemplate::index(ymuint var,
		     ymuint pos)
{
  ymuint dim = mLutTemplate->dimension();
  assert_cond( var < dim, __FILE__, __LINE__);
  ymuint n = mLutTemplate->index_num(var);
  assert_cond( pos < n, __FILE__, __LINE__);
  return mIndexArray[var][n];
}

END_NAMESPACE_YM
