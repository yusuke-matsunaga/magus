
/// @file libym_dotlib/DotlibNode.cc
/// @brief DotlibNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibNode.h"
#include "DotlibLibrary.h"
#include "DotlibCell.h"
#include "DotlibFF.h"
#include "DotlibLatch.h"
#include "DotlibPin.h"
#include "DotlibTiming.h"
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

// @brief ライブラリを表すノードから情報を取り出す．
// @param[out] library_info ライブラリの情報を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_library_info(DotlibLibrary& library_info) const
{
  library_info.init();

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

  // 'delay_model' を取り出す．
  const DotlibNode* dm_node = NULL;
  if ( !library_info.get_singleton_or_null("delay_model", dm_node) ) {
    return false;
  }
  CellLibrary::tDelayModel delay_model = CellLibrary::kDelayGenericCmos;
  if ( dm_node != NULL ) {
    ShString value = dm_node->string_value();
    if ( value == "generic_cmos" ) {
      delay_model = CellLibrary::kDelayGenericCmos;
    }
    else if ( value == "table_lookup" ) {
      delay_model = CellLibrary::kDelayTableLookup;
    }
    else if ( value == "piecewise_cmos" ) {
      delay_model = CellLibrary::kDelayPiecewiseCmos;
    }
    else if ( value == "cmos2" ) {
      delay_model = CellLibrary::kDelayCmos2;
    }
    else if ( value == "dcm" ) {
      delay_model = CellLibrary::kDelayDcm;
    }
    else {
      ostringstream buf;
      buf << value << ": Illegal value for 'delay_model'."
	  << " 'generic_cmos', 'table_lookup', 'piecewise_cmos', 'cmos2' or 'dcm' are expected.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      dm_node->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      buf.str());
      return false;
    }
  }
  library_info.mDelayModel = delay_model;

  // 'bus_naming_style' を取り出す．
  if ( !library_info.get_singleton_or_null("bus_naming_style",
					   library_info.mBusNamingStyle) ) {
    return false;
  }

  // 'comment' を取り出す．
  if ( !library_info.get_singleton_or_null("comment",
					   library_info.mComment) ) {
    return false;
  }

  // 'date' を取り出す．
  if ( !library_info.get_singleton_or_null("date",
					   library_info.mDate) ) {
    return false;
  }

  // 'revision' を取り出す．
  if ( !library_info.get_singleton_or_null("revision",
					   library_info.mRevision) ) {
    return false;
  }

  // 'current_unit' を取り出す．
  if ( !library_info.get_singleton_or_null("current_unit",
					   library_info.mCurrentUnit) ) {
    return false;
  }

  // 'leakage_power_unit' を取り出す．
  if ( !library_info.get_singleton_or_null("leakage_power_unit",
					   library_info.mLeakagePowerUnit) ) {
    return false;
  }

  // 'pulling_resistance_unit' を取り出す．
  if ( !library_info.get_singleton_or_null("pulling_resistance_unit",
					   library_info.mPullingResistanceUnit) ) {
    return false;
  }

  // 'time_unit' を取り出す．
  if ( !library_info.get_singleton_or_null("time_unit",
					   library_info.mTimeUnit) ) {
    return false;
  }

  // 'voltage_unit' を取り出す．
  if ( !library_info.get_singleton_or_null("voltage_unit",
					   library_info.mVoltageUnit) ) {
    return false;
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
  cell_info.init();

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

  // ff を取り出す．
  if ( !cell_info.get_singleton_or_null("ff", cell_info.mFF) ) {
    return false;
  }

  // latch を取り出す．
  if ( !cell_info.get_singleton_or_null("latch", cell_info.mLatch) ) {
    return false;
  }

  // statetable を取り出す．
  if ( !cell_info.get_singleton_or_null("statetable", cell_info.mStateTable) ) {
    return false;
  }

  // ff と latch と statetable は排他的
  {
    const char* first_name = NULL;
    const char* last_name = NULL;
    FileRegion first_loc;
    FileRegion last_loc;
    if ( cell_info.mFF && cell_info.mLatch ) {
      FileRegion ff_loc = cell_info.mFF->loc();
      FileRegion latch_loc = cell_info.mLatch->loc();
      if ( ff_loc.start_line() < latch_loc.start_line() ) {
	first_name = "ff";
	first_loc = ff_loc;
	last_name = "latch";
	last_loc = latch_loc;
      }
      else {
	first_name = "latch";
	first_loc = latch_loc;
	last_name = "ff";
	last_loc = ff_loc;
      }
    }
    else if ( cell_info.mFF && cell_info.mStateTable ) {
      FileRegion ff_loc = cell_info.mFF->loc();
      FileRegion st_loc = cell_info.mStateTable->loc();
      if ( ff_loc.start_line() < st_loc.start_line() ) {
	first_name = "ff";
	first_loc = ff_loc;
	last_name = "statetable";
	last_loc = st_loc;
      }
      else {
	first_name = "statetable";
	first_loc = st_loc;
	last_name = "ff";
	last_loc = ff_loc;
      }
    }
    else if ( cell_info.mLatch && cell_info.mStateTable ) {
      FileRegion latch_loc = cell_info.mLatch->loc();
      FileRegion st_loc = cell_info.mStateTable->loc();
      if ( latch_loc.start_line() < st_loc.start_line() ) {
	first_name = "latch";
	first_loc = latch_loc;
	last_name = "statetable";
	last_loc = st_loc;
      }
      else {
	first_name = "statetable";
	first_loc = st_loc;
	last_name = "latch";
	last_loc = latch_loc;
      }
    }
    if ( first_name ) {
      ostringstream buf;
      buf << "'" << last_name << "' group cannot be used with '"
	  << first_name << "' group at "
	  << first_loc << ".";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      last_loc,
		      kMsgError,
		      "DOTLIB_PARSER",
		      buf.str());
      return false;
    }
  }
  return true;
}

// @brief FFを表すノードから情報を取り出す．
// @param[out] ff_info FFの情報を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_ff_info(DotlibFF& ff_info) const
{
  ff_info.init();

  if ( !get_fl_info(ff_info) ) {
    return false;
  }

  // next_state を取り出す．
  if ( !ff_info.get_singleton("next_state", loc(), ff_info.mNextState) ) {
    return false;
  }

  // clocked_on を取り出す．
  if ( !ff_info.get_singleton("clocked_on", loc(), ff_info.mClockedOn) ) {
    return false;
  }

  // clocked_on_also を取り出す．
  if ( !ff_info.get_singleton_or_null("clocked_on_also", ff_info.mClockedOnAlso) ) {
    return false;
  }

  return true;
}

// @brief ラッチを表すノードから情報を取り出す．
// @param[out] latch_info ラッチの情報を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_latch_info(DotlibLatch& latch_info) const
{
  latch_info.init();

  if ( !get_fl_info(latch_info) ) {
    return false;
  }

  // data_in を取り出す．
  if ( !latch_info.get_singleton_or_null("data_in", latch_info.mDataIn) ) {
    return false;
  }

  // enable を取り出す．
  if ( !latch_info.get_singleton_or_null("enable", latch_info.mEnable) ) {
    return false;
  }

  // enable_also を取り出す．
  if ( !latch_info.get_singleton_or_null("enable_also", latch_info.mEnableAlso) ) {
    return false;
  }

  return true;
}

// @brief get_ff_info() と get_latch_info() の共通処理
bool
DotlibNode::get_fl_info(DotlibFL& fl_info) const
{
  // 状態変数名を得る．
  if ( !group_value()->get_string_pair(fl_info.mVar1, fl_info.mVar2) ) {
    return false;
  }

  // 属性のリストを作る．
  for (const DotlibNode* attr = attr_top(); attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    fl_info.add(attr_name, attr_value);
  }

  // clear を取り出す．
  if ( !fl_info.get_singleton_or_null("clear", fl_info.mClear) ) {
    return false;
  }

  // preset を取り出す．
  if ( !fl_info.get_singleton_or_null("preset", fl_info.mPreset) ) {
    return false;
  }

  // clear_preset_var1 を取り出す．
  const DotlibNode* tmp_node1 = NULL;
  if ( !fl_info.get_singleton_or_null("clear_preset_var1", tmp_node1) ) {
    return false;
  }
  if ( tmp_node1 ) {
    if ( !tmp_node1->is_string() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp_node1->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "String value is expected.");
      return false;
    }
    ShString str = tmp_node1->string_value();
    if ( str == "L" || str == "l" ) {
      fl_info.mClearPresetVar1 = 0;
    }
    else if ( str == "H" || str == "h" ) {
      fl_info.mClearPresetVar1 = 1;
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp_node1->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error. \"L\" or \"H\" is expected.");
      return false;
    }
  }

  // clear_preset_var2 を取り出す．
  const DotlibNode* tmp_node2 = NULL;
  if ( !fl_info.get_singleton_or_null("clear_preset_var2", tmp_node2) ) {
    return false;
  }
  if ( tmp_node2 ) {
    if ( !tmp_node2->is_string() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp_node2->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "String value is expected.");
      return false;
    }
    ShString str = tmp_node2->string_value();
    if ( str == "L" || str == "l" ) {
      fl_info.mClearPresetVar2 = 0;
    }
    else if ( str == "H" || str == "h" ) {
      fl_info.mClearPresetVar2 = 1;
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tmp_node2->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error. \"L\" or \"H\" is expected.");
      return false;
    }
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
  pin_info.init();

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
    // 'direction' がないのはエラー
    return false;
  }
  // 'direction' のハンドラは StrSimpleHandler なので文字列のはず．
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
    pin_info.mCapacitance = 0.0;
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

  // 'three_state' を取り出す．
  if ( !pin_info.get_singleton_or_null("three_state", pin_info.mThreeState) ) {
    return false;
  }

  // 'internal_node' を取り出す．
  if ( !pin_info.get_singleton_or_null("internal_node", pin_info.mInternalNode) ) {
    return false;
  }

  // 'pin_func_type' を取り出す．
  if ( !pin_info.get_singleton_or_null("pin_func_type", pin_info.mPinFuncType) ) {
    return false;
  }

  return true;
}

// @brief タイミングを表すノードから情報を取り出す．
// @param[out] timing_info タイミングの情報を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_timing_info(DotlibTiming& timing_info) const
{
  timing_info.init();

  // 属性のリストをつくる．　
  for (const DotlibNode* attr = attr_top(); attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    timing_info.add(attr_name, attr_value);
  }

  return true;
}

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

// @brief 2つの文字列からなるリストの内容を取り出す．
// @param[out] str1 1つ目の文字列を格納する変数
// @param[out] str2 2つ目の文字列を格納する変数
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibNode::get_string_pair(ShString& str1,
			    ShString& str2) const
{
  assert_cond ( is_list(), __FILE__, __LINE__);
  if ( list_size() != 2 ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Expected list size is 2.");
    return false;
  }
  const DotlibNode* node1 = top();
  assert_cond( node1 != NULL, __FILE__, __LINE__);
  if ( !node1->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    node1->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "String value is expected.");
    return false;
  }
  const DotlibNode* node2 = node1->next();
  assert_cond( node2 != NULL, __FILE__, __LINE__);
  if ( !node2->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    node2->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "String value is expected.");
    return false;
  }
  str1 = node1->string_value();
  str2 = node2->string_value();
  return true;
}

END_NAMESPACE_YM_DOTLIB
