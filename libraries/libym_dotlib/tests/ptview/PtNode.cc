
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
    mExpanded = true;
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
    mExpanded = true;
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
      if ( mNode->is_attr() ) {
	return (const char*)mNode->attr_name();
      }
    }
    else if ( column == 1 ) {
      if ( mNode->is_int() ) {
	return mNode->int_value();
      }
      else if ( mNode->is_float() ) {
	return mNode->float_value();
      }
      else if ( mNode->is_string() ) {
	return (const char*)mNode->string_value();
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
  else if ( mNode->is_group() ) {
    ymuint n = 0;
    for (const DotlibNode* node = mNode->attr_top(); node;
	 node = node->next()) {
      ++ n;
    }
    mChildren.resize(n);
    n = 0;
    for (const DotlibNode* node = mNode->attr_top(); node;
	 node = node->next()) {
      mChildren[n] = new PtNode(node);
      ++ n;
    }
  }
  else if ( mNode->is_attr() ) {
    mChildren.resize(1);
    mChildren[0] = new PtNode(mNode->attr_value());
  }
}

END_NAMESPACE_YM_DOTLIB
