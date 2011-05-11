
/// @file libym_dotlib/DotlibNode.cc
/// @brief DotlibNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibNode.h"
#include "ym_dotlib/DotlibLibrary.h"
#include "ym_dotlib/DotlibCell.h"
#include "ym_dotlib/DotlibPin.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

BEGIN_NONAMESPACE

/// @brief float 値を取り出す．
/// @note 型が違ったらエラーを MsgMgr に出力する．
bool
get_float(const DotlibNode* node,
	  double& value)
{
  if ( !node->is_float() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Float value is expected.");
    return false;
  }
  value = node->float_value();
  return true;
}

/// @brief 文字列を取り出す．
/// @note 型が違ったらエラーを MsgMgr に出力する．
bool
get_string(const DotlibNode* node,
	   ShString& value)
{
  if ( !node->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "String value is expected.");
    return false;
  }
  value = node->string_value();
  return true;
}

END_NONAMESPACE

//////////////////////////////////////////////////////////////////////
// クラス DotlibNode
//////////////////////////////////////////////////////////////////////

// @brief 1つの文字列からなるリストの場合に文字列を返す．
// @note 仮定が外れたらアボートする．
ShString
DotlibNode::get_string_from_value_list() const
{
  assert_cond( is_list(), __FILE__, __LINE__);
  assert_cond( list_size() == 1, __FILE__, __LINE__);
  const DotlibNode* value = top();
  assert_cond( value->is_string(), __FILE__, __LINE__);
  return value->string_value();
}

// @brief ライブラリを表すノードから情報を取り出す．
// @param[out] library_info ライブラリの情報を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_library_info(DotlibLibrary& library_info) const
{
  library_info.clear();

  // ライブラリ名をを得る．
  library_info.mName = group_value()->get_string_from_value_list();

  // 属性のリストを作る．
  for (const DotlibNode* attr = attr_top(); attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "cell" ) {
      library_info.mCellList.push_back(attr_value);
    }
    else {
      library_info.add(attr_name, attr_value);
    }
  }

  return true;
}

// @brief セルを表すノードから情報を取り出す．
// @param[out] cell_info セルの情報を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_cell_info(DotlibCell& cell_info) const
{
  cell_info.clear();

  // セル名を得る．
  cell_info.mName = group_value()->get_string_from_value_list();

  // 属性のリストを作る．
  for (const DotlibNode* attr = attr_top(); attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "pin" ) {
      cell_info.mPinList.push_back(attr_value);
    }
    else if ( attr_name == "bus" ) {
      cell_info.mBusList.push_back(attr_value);
    }
    else if ( attr_name == "bundle" ) {
      cell_info.mBundleList.push_back(attr_value);
    }
    else {
      cell_info.add(attr_name, attr_value);
    }
  }

  // 面積を得る．
  const DotlibNode* area_node;
  if ( !cell_info.get_singleton("area", loc(), area_node) ) {
    return false;
  }
  if ( !get_float(area_node, cell_info.mArea) ) {
    return false;
  }

  return true;
}

// @brief ピンを表すノードから情報を取り出す．
// @param[out] pin_info ピンの情報を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_pin_info(DotlibPin& pin_info) const
{
  pin_info.clear();

  // ピン名を得る．
  pin_info.mName = group_value()->get_string_from_value_list();

  // 属性のリストを作る．
  for (const DotlibNode* attr = attr_top(); attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "timing" ) {
      pin_info.mTimingList.push_back(attr_value);
    }
    else {
      pin_info.add(attr_name, attr_value);
    }
  }

  // 'direction' の翻訳をする．
  const DotlibNode* direction_node;
  if ( !pin_info.get_singleton("direction", loc(), direction_node) ) {
    return false;
  }
  if ( !direction_node->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    direction_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "String value is expected.");
    return false;
  }
  ShString value = direction_node->string_value();
  if ( value == "input" ) {
    pin_info.mDirection = DotlibPin::kInput;
  }
  else if ( value == "output" ) {
    pin_info.mDirection = DotlibPin::kOutput;
  }
  else if ( value == "inout" ) {
    pin_info.mDirection = DotlibPin::kInout;
  }
  else if ( value == "internal" ) {
    pin_info.mDirection = DotlibPin::kInternal;
  }
  else {
    ostringstream buf;
    buf << value << ": Illegal value for 'direction'."
	<< " 'input', 'output', 'inout' or 'internal' are expected.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    direction_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    buf.str());
    return false;
  }

  // 'capacitance' を取り出す．
  const DotlibNode* cap_node = NULL;
  if ( !pin_info.get_singleton_or_null("capacitance", cap_node) ) {
    return false;
  }
  if ( cap_node ) {
    if ( !get_float(cap_node, pin_info.mCapacitance) ) {
      return false;
    }
  }
  else {
    pin_info.mCapacitance = NULL;
  }

  const DotlibNode* rcap_node = NULL;
  const DotlibNode* fcap_node = NULL;
  if ( !pin_info.get_singleton_or_null("rise_capacitance", rcap_node) ) {
    return false;
  }
  if ( !pin_info.get_singleton_or_null("fall_capacitance", fcap_node) ) {
    return false;
  }
  if ( rcap_node && fcap_node ) {
    if ( !get_float(rcap_node, pin_info.mRiseCapacitance) ) {
      return false;
    }
    if ( !get_float(fcap_node, pin_info.mFallCapacitance) ) {
      return false;
    }
  }
  else {
    pin_info.mRiseCapacitance = pin_info.mCapacitance;
    pin_info.mFallCapacitance = pin_info.mCapacitance;
  }

  // 'max_fanout' を取り出す．
  const DotlibNode* max_fo_node = NULL;
  if ( !pin_info.get_singleton_or_null("max_fanout", max_fo_node) ) {
    return false;
  }
  if ( max_fo_node ) {
    if ( !get_float(max_fo_node, pin_info.mMaxFanout) ) {
      return false;
    }
  }
  else {
    pin_info.mMaxFanout = 0.0;
  }
  // 'min_fanout' を取り出す．
  const DotlibNode* min_fo_node = NULL;
  if ( !pin_info.get_singleton_or_null("min_fanout", min_fo_node) ) {
    return false;
  }
  if ( min_fo_node ) {
    if ( !get_float(min_fo_node, pin_info.mMinFanout) ) {
      return false;
    }
  }
  else {
    pin_info.mMinFanout = 0.0;
  }

  // 'max_capacitance' を取り出す．
  const DotlibNode* max_cap_node = NULL;
  if ( !pin_info.get_singleton_or_null("max_capacitance", max_cap_node) ) {
    return false;
  }
  if ( max_cap_node ) {
    if ( !get_float(max_cap_node, pin_info.mMaxCapacitance) ) {
      return false;
    }
  }
  else {
    pin_info.mMaxCapacitance = 0.0;
  }
  // 'min_capacitance' を取り出す．
  const DotlibNode* min_cap_node = NULL;
  if ( !pin_info.get_singleton_or_null("min_capacitance", min_cap_node) ) {
    return false;
  }
  if ( min_cap_node ) {
    if ( !get_float(min_cap_node, pin_info.mMinCapacitance) ) {
      return false;
    }
  }
  else {
    pin_info.mMinCapacitance = 0.0;
  }

  // 'max_transition' を取り出す．
  const DotlibNode* max_trans_node = NULL;
  if ( !pin_info.get_singleton_or_null("max_transition", max_trans_node) ) {
    return false;
  }
  if ( max_trans_node ) {
    if ( !get_float(max_trans_node, pin_info.mMaxTransition) ) {
      return false;
    }
  }
  else {
    pin_info.mMaxTransition = 0.0;
  }
  // 'min_transition' を取り出す．
  const DotlibNode* min_trans_node = NULL;
  if ( !pin_info.get_singleton_or_null("min_transition", min_trans_node) ) {
    return false;
  }
  if ( min_trans_node ) {
    if ( !get_float(min_trans_node, pin_info.mMinTransition) ) {
      return false;
    }
  }
  else {
    pin_info.mMinTransition = 0.0;
  }

  // 'function' を取り出す．
  if ( !pin_info.get_singleton_or_null("function", pin_info.mFunction) ) {
    return false;
  }

  return true;
}

END_NAMESPACE_YM_DOTLIB
