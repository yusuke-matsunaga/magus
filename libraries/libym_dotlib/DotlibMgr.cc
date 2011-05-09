
/// @file libym_dotlib/DotlibMgr.cc
/// @brief DotlibMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibMgr.h"
#include "ym_dotlib/DotlibNode.h"
#include "DotlibMgrImpl.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibMgr::DotlibMgr() :
  mImpl(new DotlibMgrImpl)
{
}

// @brief デストラクタ
DotlibMgr::~DotlibMgr()
{
  delete mImpl;
}

// @brief 初期化する．
// @note 以前，生成されたオブジェクトは破壊される．
void
DotlibMgr::clear()
{
  mImpl->clear();
}

// @brief 根のノードを返す．
const DotlibNode*
DotlibMgr::root_node() const
{
  return mImpl->root_node();
}

// @brief 使用メモリ量の一覧を出力する．
// @param[in] s 出力先のストリーム
void
DotlibMgr::show_stats(ostream& s) const
{
  mImpl->show_stats(s);
}

// @brief 1つの文字列からなる value_list から文字列を取り出す．
// @note 仮定が外れたらアボートする．
ShString
DotlibMgr::get_string(const DotlibNode* value_list)
{
  assert_cond( value_list->is_list(), __FILE__, __LINE__);
  assert_cond( value_list->list_size() == 1, __FILE__, __LINE__);
  const DotlibNode* value = value_list->top();
  assert_cond( value->is_string(), __FILE__, __LINE__);
  return value->string_value();
}

// @brief セルを表すノードから情報を取り出す．
// @param[in] cell セルを表すパース木のノード
// @param[out] name セル名を格納する変数
// @param[out] area 面積を格納する変数
// @param[out] pin_list ピングループのリスト
// @param[out] bus_list バスグループのリスト
// @param[out] bundle_list バンドルグループのリスト
// @param[out] その他の属性のリスト
// @retval true 正しく読み込めた．
// @retval false エラーが起こった．
// @note エラーは MsgMgr に出力する．
bool
DotlibMgr::get_cell_info(const DotlibNode* cell,
			 ShString& name,
			 double& area,
			 list<const DotlibNode*>& pin_list,
			 list<const DotlibNode*>& bus_list,
			 list<const DotlibNode*>& bundle_list,
			 list<const DotlibNode*>& other_list)
{
  pin_list.clear();
  bus_list.clear();
  bundle_list.clear();
  other_list.clear();

  // セル名を得る．
  name = get_string(cell->group_value());

  // 属性のリストを作る．
  const DotlibNode* area_node = NULL;
  FileRegion area_loc;
  for (const DotlibNode* attr = cell->attr_top();
       attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "area" ) {
      if ( area_node != NULL ) {
	// area 属性が2回以上現れた．
	ostringstream buf;
	buf << name << ": More than one 'area' definition."
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
      pin_list.push_back(attr_value);
    }
    else if ( attr_name == "bus" ) {
      bus_list.push_back(attr_value);
    }
    else if ( attr_name == "bundle" ) {
      bundle_list.push_back(attr_value);
    }
    else {
      other_list.push_back(attr);
    }
  }

  // 面積を得る．
  if ( area_node == NULL ) {
    ostringstream buf;
    buf << name << ": No area definition.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cell->loc(),
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
  area = area_node->float_value();

  return true;
}

END_NAMESPACE_YM_DOTLIB
