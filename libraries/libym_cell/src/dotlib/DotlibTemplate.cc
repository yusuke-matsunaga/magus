
/// @file DotlibTemplte.cc
/// @brief DotlibTemplate の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2012, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibTemplate.h"
#include "DotlibNode.h"
#include "DotlibAttr.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibTemplate
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibTemplate::DotlibTemplate()
{
}

// @brief デストラクタ
DotlibTemplate::~DotlibTemplate()
{
}

BEGIN_NONAMESPACE

bool
node_to_var(const DotlibNode* node,
	    tCellVarType &var)
{
  if ( node == NULL ) {
    var = kCellVarNone;
    return true;
  }

  ShString tmp_str = node->string_value();
  if ( tmp_str == "input_net_transition" ) {
    var = kCellVarInputNetTransition;
  }
  else if ( tmp_str == "total_output_net_capacitance" ) {
    var = kCellVarTotalOutputNetCapacitance;
  }
  else if ( tmp_str == "output_net_length" ) {
    var = kCellVarOutputNetLength;
  }
  else if ( tmp_str == "output_net_wire_cap" ) {
    var = kCellVarOutputNetWireCap;
  }
  else if ( tmp_str == "output_net_pin_cap" ) {
    var = kCellVarOutputNetPinCap;
  }
  else if ( tmp_str == "related_out_total_output_net_capacitance" ) {
    var = kCellVarRelatedOutTotalOutputNetCapacitance;
  }
  else if ( tmp_str == "related_out_output_net_length" ) {
    var = kCellVarRelatedOutOutputNetLength;
  }
  else if ( tmp_str == "related_out_output_net_wire_cap" ) {
    var = kCellVarRelatedOutOutputNetWireCap;
  }
  else if ( tmp_str == "related_out_output_net_pin_cap" ) {
    var = kCellVarRelatedOutOutputNetPinCap;
  }
  else if ( tmp_str == "constrained_pin_transition" ) {
    var = kCellVarConstrainedPinTransition;
  }
  else if ( tmp_str == "related_pin_transition" ) {
    var = kCellVarRelatedPinTransition;
  }
  else {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. illegal string for variable type.");
    return false;
  }
  return true;
}

END_NONAMESPACE

// @brief 内容をセットする．
bool
DotlibTemplate::set_data(const DotlibNode* template_node)
{
  init();

  // 名前を設定する．
  mName = template_node->group_value()->get_string_from_value_list();

  // 属性を内部のハッシュに登録する．
  for (const DotlibAttr* attr = template_node->attr_top();
       attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    add(attr_name, attr_value);
  }

  // 'variable_1' の翻訳をする．
  const DotlibNode* var1_node;
  if ( !get_singleton("variable_1", template_node->loc(), var1_node) ) {
    // 'variable_1' がないのはエラー
    return false;
  }
  if ( !node_to_var(var1_node, mVar1) ) {
    return false;
  }

  // 'variable_2' の翻訳をする．
  const DotlibNode* var2_node;
  if ( !get_singleton_or_null("variable_2", var2_node) ) {
    return false;
  }
  if ( !node_to_var(var2_node, mVar2) ) {
    return false;
  }

  // 'variable_3' の翻訳をする．
  const DotlibNode* var3_node;
  if ( !get_singleton_or_null("variable_3", var3_node) ) {
    return false;
  }
  if ( var3_node != NULL && var2_node == NULL ) {
    // variable_2 がなくて variable_3 があるのはエラー
    MsgMgr::put_msg(__FILE__, __LINE__,
		    var3_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. missing 'variable_2'.");
    return false;
  }
  if ( !node_to_var(var3_node, mVar3) ) {
    return false;
  }

  // 'index_1' を取り出す．
  const DotlibNode* index1_node;
  if ( !get_singleton("index_1", template_node->loc(), index1_node) ) {
    // index_1 がないのはエラー
    return false;
  }
  mIndex1 = index1_node->list_elem(0);

  // 'index_2' を取り出す．
  const DotlibNode* index2_node;
  if ( !get_singleton_or_null("index_2", index2_node) ) {
    return false;
  }
  if ( var2_node != NULL && index2_node == NULL ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    template_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. missing 'index_2'.");
    return false;
  }
  if ( var2_node == NULL && index2_node != NULL ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    template_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. missing 'variable_2'.");
    return false;
  }
  if ( index2_node ) {
    mIndex2 = index2_node->list_elem(0);
  }
  else {
    mIndex2 = NULL;
  }

  // 'index_3' を取り出す．
  const DotlibNode* index3_node;
  if ( !get_singleton_or_null("index_3", index3_node) ) {
    return false;
  }
  if ( var3_node != NULL && index3_node == NULL ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    template_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. missing 'index_3'.");
    return false;
  }
  if ( var3_node == NULL && index3_node != NULL ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    template_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Syntax error. missing 'variable_3'.");
    return false;
  }
  if ( index3_node ) {
    mIndex3 = index3_node->list_elem(0);
  }
  else {
    mIndex3 = NULL;
  }

  if ( var3_node != NULL ) {
    mDimension = 3;
  }
  else if ( var2_node != NULL ) {
    mDimension = 2;
  }
  else {
    mDimension = 1;
  }

  return true;
}

// @brief 名前を返す．
ShString
DotlibTemplate::name() const
{
  return mName;
}

// @brief 次元数を返す．
ymuint
DotlibTemplate::dimension() const
{
  return mDimension;
}

// @brief 'variable_1' を返す．
// @note 未定義なら kCellVarNone を返す．
tCellVarType
DotlibTemplate::variable_1() const
{
  return mVar1;
}

// @brief 'variable_2' を返す．
// @note 未定義なら kCellVarNone を返す．
tCellVarType
DotlibTemplate::variable_2() const
{
  return mVar2;
}

// @brief 'variable_3' を返す．
// @note 未定義なら kCellVarNone を返す．
tCellVarType
DotlibTemplate::variable_3() const
{
  return mVar3;
}

// @brief 'index_1' を返す．
// @note 未定義なら NULL を返す．
const DotlibNode*
DotlibTemplate::index_1() const
{
  return mIndex1;
}

/// @brief 'index_2' を返す．
// @note 未定義なら NULL を返す．
const DotlibNode*
DotlibTemplate::index_2() const
{
  return mIndex2;
}

// @brief 'index_3' を返す．
// @note 未定義なら NULL を返す．
const DotlibNode*
DotlibTemplate::index_3() const
{
  return mIndex3;
}

END_NAMESPACE_YM_DOTLIB
