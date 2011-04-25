
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
// @param[in] attr_name 属性名
// @param[in] attr_loc attr_name のファイル上の位置
PtNode::PtNode(const string& attr_name,
	       const FileRegion& attr_loc) :
  mAttrName(attr_name),
  mAttrLoc(attr_loc)
{
}

// @brief デストラクタ
PtNode::~PtNode()
{
}

// @brief 子供を追加する．
// @param[in] node 追加する子供のノード
// @note デフォルトの実装はなにもしない．
void
PtNode::add_child(PtNode* node)
{
  // っていうか呼ばれたらエラーだよね．
  assert_not_reached(__FILE__, __LINE__);
}

// @brief 属性名を表すトークンを返す．
string
PtNode::attr_name() const
{
  return mAttrName;
}

// @brief 属性名のファイル上の位置を返す．
FileRegion
PtNode::attr_loc() const
{
  return mAttrLoc;
}

// @brief 子供のノードの要素数を返す．
ymuint
PtNode::child_num() const
{
  return 0;
}

// @brief 子供のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
const PtNode*
PtNode::child(ymuint pos) const
{
  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}


//////////////////////////////////////////////////////////////////////
// クラス PtSimpleNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] attr_name 属性名
// @param[in] attr_loc attr_name のファイル上の位置
// @param[in] value 値を表すトークン
PtSimpleNode::PtSimpleNode(const string& attr_name,
			   const FileRegion& attr_loc,
			   const PtValue* value) :
  PtNode(attr_name, attr_loc),
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
// @param[in] attr_name 属性名
// @param[in] attr_loc attr_name のファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
PtComplexNode::PtComplexNode(const string& attr_name,
			     const FileRegion& attr_loc,
			     const vector<const PtValue*>& value_list) :
  PtNode(attr_name, attr_loc),
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
// @param[in] attr_name 属性名
// @param[in] attr_loc attr_name のファイル上の位置
// @param[in] value_list 値を表すトークンのリスト
PtGroupNode::PtGroupNode(const string& attr_name,
			 const FileRegion& attr_loc,
			 const vector<const PtValue*>& value_list) :
  PtComplexNode(attr_name, attr_loc, value_list)
{
}

// @brief デストラクタ
PtGroupNode::~PtGroupNode()
{
}

// @brief 子供を追加する．
// @param[in] node 追加する子供のノード
void
PtGroupNode::add_child(PtNode* node)
{
  mChildList.push_back(node);
}

// @brief 子供のノードの要素数を返す．
ymuint
PtGroupNode::child_num() const
{
  return mChildList.size();
}

// @brief 子供のノードを返す．
// @param[in] pos 位置番号 ( 0 <= pos < child_num() )
const PtNode*
PtGroupNode::child(ymuint pos) const
{
  return mChildList[pos];
}

END_NAMESPACE_YM_CELL_DOTLIB
