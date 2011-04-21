
/// @file libym_cell/dotlib/PtNode.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"


BEGIN_NAMESPACE_YM_CELL_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtNode
//////////////////////////////////////////////////////////////////////

// @brief simple attribute 用のコンストラクタ
// @param[in] attr_name 属性名を表すトークン
// @param[in] value 値を表すトークン
PtNode::PtNode(Token attr_name,
	       Token value) :
  mAttrName(attr_name),
  mValueList(1, value)
{
}

// @brief complex attribute/group statement 用のコンストラクタ
// @param[in] attr_name 属性名を表すトークン
// @param[in] value 値を表すトークンのリスト
PtNode::PtNode(Token attr_name,
	       const vector<Token>& value_list) :
  mAttrName(attr_name),
  mValueList(value_list)
{
}

// @brief デストラクタ
PtNode::~PtNode()
{
}

// @brief 子供を追加する．
// @param[in] node 追加する子供のノード
void
PtNode::add_child(PtNode* node)
{
  mChildList.push_back(node);
}

// @brief 属性名を表すトークンを返す．
Token
PtNode::attr_name() const
{
  return mAttrName;
}

// @brief 値の数を返す．
// @note simple attribute なら常に1
ymuint
PtNode::value_num() const
{
  return mValueList.size();
}

// @brief 値を表すトークンを返す．
// @param[in] pos 位置番号 ( 0 <= pos < value_num() )
Token
PtNode::value(ymuint pos) const
{
  return mValueList[pos];
}

// @brief 子供のノードの要素数を返す．
ymuint
PtNode::child_num() const
{
  return mChildList.size();
}

// @brief 子供のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
const PtNode*
PtNode::child(ymuint pos) const
{
  return mChildList[pos];
}

END_NAMESPACE_YM_CELL_DOTLIB
