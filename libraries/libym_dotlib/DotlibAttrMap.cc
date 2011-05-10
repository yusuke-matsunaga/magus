
/// @file libym_dotlib/DotlibAttrMap.cc
/// @brief DotlibAttrMap の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibAttrMap.h"


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
void
DotlibAttrMap::get(const ShString& name,
		   vector<const DotlibNode*>& node_list) const
{
  node_list.clear();
  hash_map<ShString, list<const DotlibNode*> >::const_iterator p
    = mHash.find(name);
  if ( p != mHash.end() ) {
    const list<const DotlibNode*>& src_node_list = p->second;
    node_list.reserve(src_node_list.size());
    for (list<const DotlibNode*>::const_iterator q = src_node_list.begin();
	 q != src_node_list.end(); ++ q) {
      node_list.push_back(*q);
    }
  }
}

// @brief 値を追加する．
// @param[in] name 属性名
// @param[in] node 値を表すノード
void
DotlibAttrMap::add(const ShString& name,
		   const DotlibNode* node)
{
  hash_map<ShString, list<const DotlibNode*> >::iterator p = mHash.find(name);
  if ( p == mHash.end() ) {
    mHash.insert(make_pair(name, list<const DotlibNode*>()));
    p = mHash.find(name);
  }
  (p->second).push_back(node);
}


END_NAMESPACE_YM_DOTLIB
