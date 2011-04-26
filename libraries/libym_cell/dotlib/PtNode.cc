
/// @file libym_cell/dotlib/PtNode.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "PtNodeImpl.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtNode
//////////////////////////////////////////////////////////////////////

// @brief simple attribute 用のコンストラクタ
PtNode::PtNode()
{
}

// @brief デストラクタ
PtNode::~PtNode()
{
}

// @brief 子供を追加する．
// @param[in] attr_name 属性名
// @param[in] node 追加する子供のノード
// @note デフォルトの実装はなにもしない．
void
PtNode::add_child(const ShString& attr_name,
		  PtNode* node)
{
  // っていうか呼ばれたらエラーだよね．
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 子供の属性名の個数を返す．
// @note デフォルトの実装は 0 を返す．
ymuint
PtNode::child_attr_num() const
{
  return 0;
}

// @brief 子供の属性名を返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_attr_num() )
ShString
PtNode::child_attr_name(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return ShString();
}

// @brief 属性に対応した子供の要素数を返す．
// @param[in] attr_name 子供の属性名
// @note デフォルトの実装は 0 を返す．
ymuint
PtNode::child_num(const ShString& attr_name) const
{
  return 0;
}

// @brief 属性に対応した子供を返す．
// @param[in] attr_name 子供の属性名
// @param[in] pos 位置番号 ( 0 <= pos < child_num(attr_name) )
const PtNode*
PtNode::child(const ShString& attr_name,
	      ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス PtSimpleNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value 値を表すトークン
PtSimpleNode::PtSimpleNode(const PtValue* value) :
  mValue(value)
{
}

// @brief デストラクタ
PtSimpleNode::~PtSimpleNode()
{
}

// @brief 値の数を返す．
// @note simple attribute なら常に1
ymuint
PtSimpleNode::value_num() const
{
  return 1;
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtSimpleNode::value(ymuint pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return mValue;
}


//////////////////////////////////////////////////////////////////////
// クラス PtComplexNode
//////////////////////////////////////////////////////////////////////

// @brief 用のコンストラクタ
// @param[in] value_list 値を表すトークンのリスト
PtComplexNode::PtComplexNode(const vector<const PtValue*>& value_list) :
  mValueList(value_list)
{
}

// @brief デストラクタ
PtComplexNode::~PtComplexNode()
{
}

// @brief 値の数を返す．
ymuint
PtComplexNode::value_num() const
{
  return mValueList.size();
}

// @brief 値を返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
const PtValue*
PtComplexNode::value(ymuint pos) const
{
  return mValueList[pos];
}


//////////////////////////////////////////////////////////////////////
// クラス PtGroupNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] value_list 値を表すトークンのリスト
PtGroupNode::PtGroupNode(const vector<const PtValue*>& value_list) :
  PtComplexNode(value_list)
{
}

// @brief デストラクタ
PtGroupNode::~PtGroupNode()
{
}

// @brief 子供を追加する．
// @param[in] attr_name 属性名
// @param[in] node 追加する子供のノード
void
PtGroupNode::add_child(const ShString& attr_name,
		       PtNode* node)
{
#if 1
  if ( mChildMap.count(attr_name) == 0 ) {
    mChildMap.insert(make_pair(attr_name, vector<PtNode*>(1, node)));
    mAttrList.push_back(attr_name);
  }
  else {
    hash_map<ShString, vector<PtNode*> >::iterator p = mChildMap.find(attr_name);
    (p->second).push_back(node);
  }
#endif
}

// @brief 子供の属性名の個数を返す．
ymuint
PtGroupNode::child_attr_num() const
{
#if 1
  return mAttrList.size();
#else
  return 0;
#endif
}

// @brief 子供の属性名を返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_attr_num() )
ShString
PtGroupNode::child_attr_name(ymuint pos) const
{
#if 1
  assert_cond( pos < child_attr_num(), __FILE__, __LINE__);
  return mAttrList[pos];
#else
  return ShString();
#endif
}

// @brief 属性に対応した子供の要素数を返す．
// @param[in] attr_name 子供の属性名
// @note デフォルトの実装は 0 を返す．
ymuint
PtGroupNode::child_num(const ShString& attr_name) const
{
#if 1
  hash_map<ShString, vector<PtNode*> >::const_iterator p
    = mChildMap.find(attr_name);
  const vector<PtNode*>& child_list = p->second;
  return child_list.size();
#else
  return 0;
#endif
}

// @brief 属性に対応した子供を返す．
// @param[in] attr_name 子供の属性名
// @param[in] pos 位置番号 ( 0 <= pos < child_num(attr_name) )
const PtNode*
PtGroupNode::child(const ShString& attr_name,
		   ymuint pos) const
{
#if 1
  hash_map<ShString, vector<PtNode*> >::const_iterator p
    = mChildMap.find(attr_name);
  const vector<PtNode*>& child_list = p->second;
  assert_cond( pos < child_list.size(), __FILE__, __LINE__);
  return child_list[pos];
#else
  return NULL;
#endif
}

END_NAMESPACE_YM_CELL_DOTLIB
