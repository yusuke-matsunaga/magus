
/// @file libym_verilog/tests/ptview/PtNode.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "PtBaseNode.h"
#include "PtRootNode.h"
#include "ym_dotlib/DotlibNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

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

// @brief 親のインデックスを返す．
QModelIndex
PtNode::parent_index() const
{
  return mParentIndex;
}

//////////////////////////////////////////////////////////////////////
// クラス PtBaseNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node 対応するパース木のノード
PtBaseNode::PtBaseNode(const DotlibNode* node) :
  mNode(node),
  mExpanded(false)
{
}

// @brief デストラクタ
PtBaseNode::~PtBaseNode()
{
  for (vector<PtNode*>::iterator p = mChildren.begin();
       p != mChildren.end(); ++ p) {
    delete *p;
  }
}

// @brief 子供の数を返す．
int
PtBaseNode::child_num() const
{
  if ( !mExpanded ) {
    expand();
  }
  return mChildren.size();
}

// @brief 子供を返す．
// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
PtNode*
PtBaseNode::child(int pos) const
{
  if ( !mExpanded ) {
    expand();
  }
  return mChildren[pos];
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
PtBaseNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      if ( mNode->is_group() ) {
      }
      else if ( mNode->is_attr() ) {
	return (const char*)mNode->attr_name();
      }
    }
    else if ( column == 1 ) {
      if ( mNode->is_attr() ) {
	const DotlibNode* node = mNode->attr_value();
	if ( node->is_int() ) {
	  return node->int_value();
	}
	else if ( node->is_float() ) {
	  return node->float_value();
	}
	else if ( node->is_string() ) {
	  return (const char*)node->string_value();
	}
	else if ( node->is_group() ) {
	  const DotlibNode* value = node->group_value();
	  if ( value->list_size() == 1 ) {
	    return (const char*)value->top()->string_value();
	  }
	  else {
	    ostringstream buf;
	    const char* comma = "";
	    for (const DotlibNode* node = value->top();
		 node; node = node->next()) {
	      buf << comma;
	      if ( node->is_int() ) {
		buf << node->int_value();
	      }
	      else if ( node->is_float() ) {
		buf << node->float_value();
	      }
	      else if ( node->is_string() ) {
		buf << (const char*)node->string_value();
	      }
	      comma = ", ";
	    }
	  }
	}
      }
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
PtBaseNode::loc() const
{
  return mNode->loc();
}

// @brief 子供の配列を作る．
void
PtBaseNode::expand() const
{
  if ( mNode->is_list() ) {
    ymuint n = mNode->list_size();
    mChildren.resize(n);
    ymuint i = 0;
    for (const DotlibNode* node = mNode->top(); node; node = node->next()) {
      mChildren[i] = new PtBaseNode(node);
      ++ i;
    }
  }
  else if ( mNode->is_attr() ) {
    const DotlibNode* node = mNode->attr_value();
    if ( node->is_group() ) {
      ymuint n = 0;
      for (const DotlibNode* node1 = node->attr_top(); node1;
	   node1 = node1->next()) {
	++ n;
      }
      mChildren.resize(n);
      n = 0;
      for (const DotlibNode* node1 = node->attr_top(); node1;
	   node1 = node1->next()) {
	mChildren[n] = new PtBaseNode(node1);
	++ n;
      }
    }
  }
  mExpanded = true;
}


//////////////////////////////////////////////////////////////////////
// クラス PtRootNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node 対応するパース木のノード
PtRootNode::PtRootNode(const DotlibNode* node) :
  mRoot(new PtBaseNode(node))
{
}

// @brief デストラクタ
PtRootNode::~PtRootNode()
{
  delete mRoot;
}

// @brief 子供の数を返す．
int
PtRootNode::child_num() const
{
  return 1;
}

// @brief 子供を返す．
// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
PtNode*
PtRootNode::child(int pos) const
{
  assert_cond( pos == 0, __FILE__, __LINE__);
  return mRoot;
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
PtRootNode::data(int column,
		 int role) const
{
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
PtRootNode::loc() const
{
  return mRoot->loc();
}

END_NAMESPACE_YM_DOTLIB
