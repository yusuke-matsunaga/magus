
/// @file DotlibPin.cc
/// @brief DotlibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibPin.h"
#include "DotlibNode.h"
#include "DotlibAttr.h"
#include "utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibPin::DotlibPin()
{
}

// @brief デストラクタ
DotlibPin::~DotlibPin()
{
}

// @brief 内容をセットする．
bool
DotlibPin::set_data(const DotlibNode* pin_node)
{
  init();

  mFunction = NULL;
  mThreeState = NULL;
  mInternalNode = NULL;
  mPinFuncType = NULL;

  mTimingList.clear();

  // 名前のリストを設定する．
  const DotlibNode* list_node = pin_node->group_value();
  ymuint n = list_node->list_size();
  mNameList.clear();
  mNameList.resize(n);
  for (ymuint i = 0; i < n; ++ i) {
    const DotlibNode* str_node = list_node->list_elem(i);
    if ( !str_node->is_string() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      str_node->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error. string value expected");
      return false;
    }
    mNameList[i] = str_node->string_value();
  }

  // 属性のリストを作る．
  for (const DotlibAttr* attr = pin_node->attr_top();
       attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "timing" ) {
      mTimingList.push_back(attr_value);
    }
    else {
      add(attr_name, attr_value);
    }
  }

  // 'direction' の翻訳をする．
  const DotlibNode* direction_node;
  if ( !get_singleton("direction", pin_node->loc(), direction_node) ) {
    // 'direction' がないのはエラー
    return false;
  }
  // 'direction' のハンドラは StrSimpleHandler なので文字列のはず．
  ShString value = direction_node->string_value();
  if ( value == "input" ) {
    mDirection = DotlibPin::kInput;
  }
  else if ( value == "output" ) {
    mDirection = DotlibPin::kOutput;
  }
  else if ( value == "inout" ) {
    mDirection = DotlibPin::kInout;
  }
  else if ( value == "internal" ) {
    mDirection = DotlibPin::kInternal;
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
  if ( !get_singleton_or_null("capacitance", cap_node) ) {
    return false;
  }
  if ( cap_node ) {
    if ( !cap_node->get_float(mCapacitance) ) {
      return false;
    }
  }
  else {
    mCapacitance = 0.0;
  }

  const DotlibNode* rcap_node = NULL;
  const DotlibNode* fcap_node = NULL;
  if ( !get_singleton_or_null("rise_capacitance", rcap_node) ) {
    return false;
  }
  if ( !get_singleton_or_null("fall_capacitance", fcap_node) ) {
    return false;
  }
  if ( rcap_node && fcap_node ) {
    if ( !rcap_node->get_float(mRiseCapacitance) ) {
      return false;
    }
    if ( !fcap_node->get_float(mFallCapacitance) ) {
      return false;
    }
  }
  else {
    mRiseCapacitance = mCapacitance;
    mFallCapacitance = mCapacitance;
  }

  // 'max_fanout' を取り出す．
  const DotlibNode* max_fo_node = NULL;
  if ( !get_singleton_or_null("max_fanout", max_fo_node) ) {
    return false;
  }
  if ( max_fo_node ) {
    if ( !max_fo_node->get_float(mMaxFanout) ) {
      return false;
    }
  }
  else {
    mMaxFanout = 0.0;
  }
  // 'min_fanout' を取り出す．
  const DotlibNode* min_fo_node = NULL;
  if ( !get_singleton_or_null("min_fanout", min_fo_node) ) {
    return false;
  }
  if ( min_fo_node ) {
    if ( !min_fo_node->get_float(mMinFanout) ) {
      return false;
    }
  }
  else {
    mMinFanout = 0.0;
  }

  // 'max_capacitance' を取り出す．
  const DotlibNode* max_cap_node = NULL;
  if ( !get_singleton_or_null("max_capacitance", max_cap_node) ) {
    return false;
  }
  if ( max_cap_node ) {
    if ( !max_cap_node->get_float(mMaxCapacitance) ) {
      return false;
    }
  }
  else {
    mMaxCapacitance = 0.0;
  }
  // 'min_capacitance' を取り出す．
  const DotlibNode* min_cap_node = NULL;
  if ( !get_singleton_or_null("min_capacitance", min_cap_node) ) {
    return false;
  }
  if ( min_cap_node ) {
    if ( !min_cap_node->get_float(mMinCapacitance) ) {
      return false;
    }
  }
  else {
    mMinCapacitance = 0.0;
  }

  // 'max_transition' を取り出す．
  const DotlibNode* max_trans_node = NULL;
  if ( !get_singleton_or_null("max_transition", max_trans_node) ) {
    return false;
  }
  if ( max_trans_node ) {
    if ( !max_trans_node->get_float(mMaxTransition) ) {
      return false;
    }
  }
  else {
    mMaxTransition = 0.0;
  }
  // 'min_transition' を取り出す．
  const DotlibNode* min_trans_node = NULL;
  if ( !get_singleton_or_null("min_transition", min_trans_node) ) {
    return false;
  }
  if ( min_trans_node ) {
    if ( !min_trans_node->get_float(mMinTransition) ) {
      return false;
    }
  }
  else {
    mMinTransition = 0.0;
  }

  // 'function' を取り出す．
  if ( !get_singleton_or_null("function", mFunction) ) {
    return false;
  }

  // 'three_state' を取り出す．
  if ( !get_singleton_or_null("three_state", mThreeState) ) {
    return false;
  }

  // 'internal_node' を取り出す．
  if ( !get_singleton_or_null("internal_node", mInternalNode) ) {
    return false;
  }

  // 'pin_func_type' を取り出す．
  if ( !get_singleton_or_null("pin_func_type", mPinFuncType) ) {
    return false;
  }

  return true;
}

// @brief 名前のリストの要素数を返す．
ymuint
DotlibPin::num() const
{
  return mNameList.size();
}

// @brief 名前を返す．
// @param[in] pos 位置番号 ( 0 <= pos < num() )
ShString
DotlibPin::name(ymuint pos) const
{
  assert_cond( pos < num(), __FILE__, __LINE__);
  return mNameList[pos];
}

// @brief "direction" を返す．
DotlibPin::tDirection
DotlibPin::direction() const
{
  return mDirection;
}

// @brief "capacitance" を返す．
double
DotlibPin::capacitance() const
{
  return mCapacitance;
}

// @brief "rise_capacitance" を返す．
double
DotlibPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief "fall_capacitance" を返す．
double
DotlibPin::fall_capacitance() const
{
  return mFallCapacitance;
}

// @brief "max_fanout" を返す．
double
DotlibPin::max_fanout() const
{
  return mMaxFanout;
}

// @brief "min_fanout" を返す．
double
DotlibPin::min_fanout() const
{
  return mMinFanout;
}

// @brief "max_capacitance" を返す．
double
DotlibPin::max_capacitance() const
{
  return mMaxCapacitance;
}

// @brief "min_capacitance" を返す．
double
DotlibPin::min_capacitance() const
{
  return mMinCapacitance;
}

// @brief "max_transition" を返す．
double
DotlibPin::max_transition() const
{
  return mMaxTransition;
}

// @brief "min_transition" を返す．
double
DotlibPin::min_transition() const
{
  return mMinTransition;
}

// @brief "function" を返す．
const DotlibNode*
DotlibPin::function() const
{
  return mFunction;
}

// @brief "three_state" を返す．
const DotlibNode*
DotlibPin::three_state() const
{
  return mThreeState;
}

// @brief "internal_node" を返す．
const DotlibNode*
DotlibPin::internal_node() const
{
  return mInternalNode;
}

// @brief "pin_func_type" を返す．
const DotlibNode*
DotlibPin::pin_func_type() const
{
  return mPinFuncType;
}

// @brief "timing" グループのリストを得る．
const list<const DotlibNode*>&
DotlibPin::timing_list() const
{
  return mTimingList;
}

END_NAMESPACE_YM_DOTLIB
