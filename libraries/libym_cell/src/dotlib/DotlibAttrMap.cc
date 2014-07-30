
/// @file DotlibAttrMap.cc
/// @brief DotlibAttrMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibAttrMap.h"
#include "DotlibNode.h"
#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibAttrMap
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibAttrMap::DotlibAttrMap()
{
}

// @brief デストラクタ
DotlibAttrMap::~DotlibAttrMap()
{
}

// @brief 値を得る．
// @param[in] name 属性名
// @param[out] node_list 値のリストを格納する変数
// @retval true その属性値が定義されていた．
// @retval false その属性値が定義されていなかった．
bool
DotlibAttrMap::get(const char* name,
		   vector<const DotlibNode*>& node_list) const
{
  node_list.clear();
  StrNodeListMap::const_iterator p
    = mHash.find(ShString(name));
  if ( p == mHash.end() ) {
    return false;
  }
  const list<const DotlibNode*>& src_node_list = p->second;
  node_list.reserve(src_node_list.size());
  for (list<const DotlibNode*>::const_iterator q = src_node_list.begin();
       q != src_node_list.end(); ++ q) {
    node_list.push_back(*q);
  }
  return true;
}

// @brief 値が単一と仮定してそのノードを返す．
// @param[in] name 属性名
// @param[in] loc 属性全体のファイル位置(エラー出力用)
// @param[out] node 結果のノードを格納するノード
// @retval true 値の読み出しが成功した．
// @retval false エラーが起こった．
bool
DotlibAttrMap::get_singleton(const char* name,
			     const FileRegion& loc,
			     const DotlibNode*& node) const
{
  if ( !get_singleton_or_null(name, node) ) {
    return false;
  }
  if ( node == NULL ) {
    ostringstream buf;
    buf << "No '" << name << "' definition.";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    loc,
		    kMsgError,
		    "DOTLIB_PARSER",
		    buf.str());
    return false;
  }
  return true;
}

// @brief 値が単一と仮定してそのノードを返す．
// @param[in] name 属性名
// @param[out] node 結果のノードを格納するノード
// @retval true 値の読み出しが成功した．
// @retval false エラーが起こった．
bool
DotlibAttrMap::get_singleton_or_null(const char* name,
				     const DotlibNode*& node) const
{
  StrNodeListMap::const_iterator p
    = mHash.find(ShString(name));
  if ( p == mHash.end() ) {
    node = NULL;
    return true;
  }

  const list<const DotlibNode*>& node_list = p->second;
  list<const DotlibNode*>::const_iterator q = node_list.begin();
  node = *q;
  ++ q;
  if ( q != node_list.end() ) {
    const DotlibNode* second = *q;
    ostringstream buf;
    buf << "More than one '" << name << "' definitions."
	<< " First occurence is " << node->loc() << ".";
    MsgMgr::put_msg(__FILE__, __LINE__,
		    second->loc(),
		    kMsgError,
		    "DOTLIB_PARSER",
		    buf.str());
    return false;
  }
  return true;
}

// @brief 内容を初期化する．
void
DotlibAttrMap::init()
{
  mHash.clear();
}

// @brief 値を追加する．
// @param[in] name 属性名
// @param[in] node 値を表すノード
void
DotlibAttrMap::add(const ShString& name,
		   const DotlibNode* node)
{
  StrNodeListMap::iterator p = mHash.find(name);
  if ( p == mHash.end() ) {
    mHash.insert(make_pair(name, list<const DotlibNode*>()));
    p = mHash.find(name);
  }
  (p->second).push_back(node);
}


END_NAMESPACE_YM_DOTLIB
