
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
  const DotlibNode* area_node = NULL;
  FileRegion area_loc;
  for (const DotlibNode* attr = attr_top(); attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "area" ) {
      if ( area_node != NULL ) {
	// area 属性が2回以上現れた．
	ostringstream buf;
	buf << cell_info.name() << ": More than one 'area' definition."
	    << " First occurence is " << area_loc << ".";
	MsgMgr::put_msg(__FILE__, __LINE__,
			attr->loc(),
			kMsgError,
			"DOTLIB_PARSER",
			buf.str());
	return false;
      }
      area_node = attr_value;
      area_loc = attr->loc();
    }
    else if ( attr_name == "pin" ) {
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
  if ( area_node == NULL ) {
    ostringstream buf;
    buf << cell_info.name() << ": No area definition.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    buf.str());
    return false;
  }
  if ( !area_node->is_float() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    area_node->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    "Float value is expected.");
    return false;
  }
  cell_info.mArea = area_node;

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

  return true;
}

END_NAMESPACE_YM_DOTLIB
