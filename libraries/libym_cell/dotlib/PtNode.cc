
/// @file libym_cell/dotlib/PtNode.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "PtValue.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
PtNode::PtNode()
{
}

// @brief デストラクタ
PtNode::~PtNode()
{
}

// @brief attribute を設定する．
// @param[in] attr_name 属性名
// @param[in] value 値
// @return 設定が失敗したら false を返す．
bool
PtNode::add_attr(const ShString& attr_name,
		 PtValue* value)
{
  hash_map<ShString, PtValue*>::iterator p = mAttrMap.find(attr_name);
  if ( p == mAttrMap.end() ) {
    mAttrMap.insert(make_pair(attr_name, value));
  }
  else {
    PtValue* value1 = p->second;
    assert_cond( value1 != NULL, __FILE__, __LINE__);
    PtValue* end = value1;
    // 実はかなり効率が悪い
    for ( ; end->mNext != NULL; end = end->mNext) ;
    end->mNext = value;
  }
  return true;
}

// @brief attribute を取得する．
// @param[in] attr_name 属性名
// @note 該当する属性がなければ NULL を返す．
const PtValue*
PtNode::get_attr(const ShString& attr_name) const
{
  hash_map<ShString, PtValue*>::const_iterator p = mAttrMap.find(attr_name);
  if ( p == mAttrMap.end() ) {
    return NULL;
  }
  return p->second;
}

END_NAMESPACE_YM_CELL_DOTLIB
