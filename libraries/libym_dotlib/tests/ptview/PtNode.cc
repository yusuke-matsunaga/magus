
/// @file libym_verilog/tests/ptview/PtNode.cc
/// @brief PtNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: PtNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "PtNode.h"
#include "ym_dotlib/DotlibNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス PtNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] node 対応するパース木のノード
PtNode::PtNode(const DotlibNode* node) :
  mNode(node),
  mExpanded(false)
{
}

// @brief デストラクタ
PtNode::~PtNode()
{
  for (vector<PtNode*>::iterator p = mChildren.begin();
       p != mChildren.end(); ++ p) {
    delete *p;
  }
}

// @brief 親のインデックスを返す．
QModelIndex
PtNode::parent_index() const
{
  return mParentIndex;
}

// @brief 子供の数を返す．
int
PtNode::child_num() const
{
  if ( !mExpanded ) {
    expand();
  }
  return mChildren.size();
}

// @brief 子供を返す．
// @param[in] pos 位置番号 ( 0 <= pos < row_num() )
PtNode*
PtNode::child(int pos) const
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
PtNode::data(int column,
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
PtNode::loc() const
{
  return mNode->loc();
}

// @brief 子供の配列を作る．
void
PtNode::expand() const
{
  if ( mNode->is_list() ) {
    ymuint n = mNode->list_size();
    mChildren.resize(n);
    ymuint i = 0;
    for (const DotlibNode* node = mNode->top(); node; node = node->next()) {
      mChildren[i] = new PtNode(node);
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
	mChildren[n] = new PtNode(node1);
	++ n;
      }
    }
  }
  mExpanded = true;
}

END_NAMESPACE_YM_DOTLIB
