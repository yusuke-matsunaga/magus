
/// @file libym_dotlib/DotlibCell.cc
/// @brief DotlibCell の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibCell.h"
#include "DotlibNode.h"
#include "DotlibAttr.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibCell
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibCell::DotlibCell()
{
}

// @brief デストラクタ
DotlibCell::~DotlibCell()
{
}

// @brief 内容を設定する．
bool
DotlibCell::set_data(const DotlibNode* cell_node)
{
  init();

  mArea = 0.0;
  mBusNamingStyle = NULL;
  mFF = NULL;
  mLatch = NULL;
  mStateTable = NULL;

  mPinList.clear();
  mBusList.clear();
  mBundleList.clear();

  // セル名を得る．
  mName = cell_node->group_value()->get_string_from_value_list();

  // 属性のリストを作る．
  for (const DotlibAttr* attr = cell_node->attr_top();
       attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "pin" ) {
      mPinList.push_back(attr_value);
    }
    else if ( attr_name == "bus" ) {
      mBusList.push_back(attr_value);
    }
    else if ( attr_name == "bundle" ) {
      mBundleList.push_back(attr_value);
    }
    else {
      add(attr_name, attr_value);
    }
  }

  // 面積を得る．
  const DotlibNode* area_node;
  if ( !get_singleton("area", cell_node->loc(), area_node) ) {
    return false;
  }
  if ( !area_node->get_float(mArea) ) {
    return false;
  }

  // ff を取り出す．
  if ( !get_singleton_or_null("ff", mFF) ) {
    return false;
  }

  // latch を取り出す．
  if ( !get_singleton_or_null("latch", mLatch) ) {
    return false;
  }

  // statetable を取り出す．
  if ( !get_singleton_or_null("statetable", mStateTable) ) {
    return false;
  }

  // ff と latch と statetable は排他的
  {
    const char* first_name = NULL;
    const char* last_name = NULL;
    FileRegion first_loc;
    FileRegion last_loc;
    if ( mFF && mLatch ) {
      FileRegion ff_loc = mFF->loc();
      FileRegion latch_loc = mLatch->loc();
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
    else if ( mFF && mStateTable ) {
      FileRegion ff_loc = mFF->loc();
      FileRegion st_loc = mStateTable->loc();
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
    else if ( mLatch && mStateTable ) {
      FileRegion latch_loc = mLatch->loc();
      FileRegion st_loc = mStateTable->loc();
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

// @brief 名前を返す．
ShString
DotlibCell::name() const
{
  return mName;
}

// @brief 面積を返す．
double
DotlibCell::area() const
{
  return mArea;
}

// @brief ピングループのリストを返す．
const list<const DotlibNode*>&
DotlibCell::pin_list() const
{
  return mPinList;
}

// @brief バスグループのリストを返す．
const list<const DotlibNode*>&
DotlibCell::bus_list() const
{
  return mBusList;
}

// @brief バンドルグループのリストを返す．
const list<const DotlibNode*>&
DotlibCell::bundle_list() const
{
  return mBundleList;
}

// @brief ff グループを返す．
// @note なければ NULL を返す．
const DotlibNode*
DotlibCell::ff() const
{
  return mFF;
}

// @brief latch グループを返す．
// @note なければ NULL を返す．
const DotlibNode*
DotlibCell::latch() const
{
  return mLatch;
}

// @brief statetable グループを返す．
// @note なければ NULL を返す．
const DotlibNode*
DotlibCell::statetable() const
{
  return mStateTable;
}

END_NAMESPACE_YM_DOTLIB
