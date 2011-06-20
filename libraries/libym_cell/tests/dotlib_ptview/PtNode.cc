
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
#include "dotlib/DotlibNode.h"


BEGIN_NAMESPACE_YM_DOTLIB

BEGIN_NONAMESPACE

const char*
opr_str(DotlibNode::tType type)
{
  switch ( type ) {
  case DotlibNode::kPlus:  return "+";
  case DotlibNode::kMinus: return "-";
  case DotlibNode::kMult:  return "*";
  case DotlibNode::kDiv:   return "/";
  default: break;
  }
  assert_not_reached(__FILE__, __LINE__);
  return "";
}

void
vs_sub(ostream& s,
       const DotlibNode* node)
{
  if ( node->is_int() ) {
    s << node->int_value();
  }
  else if ( node->is_float() ) {
    s << node->float_value();
  }
  else if ( node->is_string() ) {
    s << node->string_value();
  }
  else if ( node->is_opr() ) {
    s << "( ";
    vs_sub(s, node->opr1());
    s << " " << opr_str(node->type()) << " ";
    vs_sub(s, node->opr2());
    s << " )";
  }
  else if ( node->is_list() ) {
    const char* comma = "";
    s << "(";
    for (const DotlibNode* node1 = node->top(); node1;
	 node1 = node1->next()) {
      s << comma;
      vs_sub(s, node1);
      comma = ", ";
    }
    s << ")";
  }
  else if ( node->is_group() ) {
    vs_sub(s, node->group_value());
  }
}

string
value_str(const DotlibNode* node)
{
  ostringstream buf;
  vs_sub(buf, node);
  return buf.str();
}

END_NONAMESPACE


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
    if ( mNode->is_int() ) {
      if ( column == 0 ) {
	return "int";
      }
      else if ( column == 1 ) {
	return mNode->int_value();
      }
    }
    else if ( mNode->is_float() ) {
      if ( column == 0 ) {
	return "float";
      }
      else if ( column == 1 ) {
	return mNode->float_value();
      }
    }
    else if ( mNode->is_string() ) {
      if ( column == 0 ) {
	return "string";
      }
      else if ( column == 1 ) {
	return (const char*)mNode->string_value();
      }
    }
    else if ( mNode->is_opr() ) {
      if ( column == 0 ) {
	return opr_str(mNode->type());
      }
    }
    else if ( mNode->is_list() ) {
      if ( column == 0 ) {
	return "list";
      }
      else if ( column == 1 ) {
	string str = value_str(mNode);
	return QVariant(str.c_str());
      }
    }
    else if ( mNode->is_group() ) {
      if ( column == 0 ) {
	return "value";
      }
      else if ( column == 1 ) {
	string str = value_str(mNode->group_value());
	return QVariant(str.c_str());
      }
    }
    else if ( mNode->is_attr() ) {
      if ( column == 0 ) {
	return (const char*)mNode->attr_name();
      }
      else if ( column == 1 ) {
	string str = value_str(mNode->attr_value());
	return QVariant(str.c_str());
      }
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
PtBaseNode::loc() const
{
  if ( mNode->is_group() ) {
    return mNode->group_value()->loc();
  }
  else {
    return mNode->loc();
  }
}

// @brief 子供の配列を作る．
void
PtBaseNode::expand() const
{
  if ( mNode->is_opr() ) {
    mChildren.resize(2);
    mChildren[0] = new PtBaseNode(mNode->opr1());
    mChildren[1] = new PtBaseNode(mNode->opr2());
  }
  else if ( mNode->is_list() ) {
    ymuint n = mNode->list_size();
    mChildren.resize(n);
    ymuint i = 0;
    for (const DotlibNode* node = mNode->top(); node; node = node->next()) {
      mChildren[i] = new PtBaseNode(node);
      ++ i;
    }
  }
  else if ( mNode->is_group() ) {
    mChildren.resize(1);
    mChildren[0] = new PtBaseNode(mNode->group_value());
  }
  else if ( mNode->is_attr() ) {
    const DotlibNode* value = mNode->attr_value();
    if ( value->is_group() ) {
      ymuint n = 1;
      for (const DotlibNode* node1 = value->attr_top(); node1;
	   node1 = node1->next()) {
	++ n;
      }
      mChildren.resize(n);
      mChildren[0] = new PtBaseNode(value);
      n = 1;
      for (const DotlibNode* node1 = value->attr_top(); node1;
	   node1 = node1->next()) {
	mChildren[n] = new PtBaseNode(node1);
	++ n;
      }
    }
    else {
      mChildren.resize(1);
      mChildren[0] = new PtBaseNode(value);
    }
  }
  mExpanded = true;
}

END_NAMESPACE_YM_DOTLIB
