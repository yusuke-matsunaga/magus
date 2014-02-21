
/// @file DotlibNode.cc
/// @brief DotlibNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibNode.h"
#include "utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

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
  const DotlibNode* value = list_elem(0);
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
  const DotlibNode* node1 = list_elem(0);
  assert_cond( node1 != NULL, __FILE__, __LINE__);
  if ( !node1->is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    node1->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "String value is expected.");
    return false;
  }
  const DotlibNode* node2 = list_elem(1);
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

// @brief float 値を取り出す．
// @note 型が違ったらエラーを MsgMgr に出力する．
bool
DotlibNode::get_float(double& value) const
{
  if ( !is_float() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Float value is expected.");
    return false;
  }
  value = float_value();
  return true;
}

// @brief 文字列を取り出す．
// @note 型が違ったらエラーを MsgMgr に出力する．
bool
DotlibNode::get_string(ShString& value) const
{
  if ( !is_string() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "String value is expected.");
    return false;
  }
  value = string_value();
  return true;
}

END_NAMESPACE_YM_DOTLIB
