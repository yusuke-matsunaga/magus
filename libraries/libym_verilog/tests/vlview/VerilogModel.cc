
/// @file libym_verilog/tests/vlview/VlTokenList.cc
/// @brief VlTokenList の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlParseTreeModel.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VlParseTreeModel.h"
#include "VlPtNode.h"
#include "VlPtNode_root.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief コンストラクタ
// @param[in] parent 親のオブジェクト
VlParseTreeModel::VlParseTreeModel(QObject* parent) :
  QAbstractItemModel(parent),
  mRootNode(NULL)
{
}

// @brief デストラクタ
VlParseTreeModel::~VlParseTreeModel()
{
}
  
// @brief 対応するインデックスを返す．
// @param[in] row 行番号
// @param[in] column 列番号
// @param[in] parent 親のインデックス
QModelIndex
VlParseTreeModel::index(int row,
			int column,
			const QModelIndex& parent) const
{
  VlPtNode* node = index2node(parent);
  if ( !node ) {
    return QModelIndex();
  }
  
  VlPtNode* cnode = node->child(row);
  cnode->mParentIndex = parent;
  return createIndex(row, column, cnode);
}

// @brief 親のインデックスを返す．
// @param[in] child 子のインデックス
QModelIndex
VlParseTreeModel::parent(const QModelIndex& child) const
{
  VlPtNode* node = index2node(child);
  if ( node ) {
    return node->parent_index();
  }
  else {
    return QModelIndex();
  }
}
  
// @brief コラム数を返す．
int
VlParseTreeModel::columnCount(const QModelIndex& parent) const
{
  // ラベル, 値 の2つ
  return 2;
}
  
// @brief 行数を返す．
int
VlParseTreeModel::rowCount(const QModelIndex& parent) const
{
  VlPtNode* node = index2node(parent);
  if ( node ) {
    return node->child_num();
  }
  else {
    return 0;
  }
}
  
// @brief 該当するデータを返す．
QVariant
VlParseTreeModel::data(const QModelIndex& index,
		       int role) const
{
  VlPtNode* node = index2node(index);
  if ( node ) {
    return node->data(index.column(), role);
  }
  else {
    return QVariant();
  }
}

/// @brief ヘッダのデータを返す．
QVariant
VlParseTreeModel::headerData(int section,
			     Qt::Orientation orientation,
			     int role) const
{
  if ( orientation == Qt::Horizontal && role == Qt::DisplayRole ) {
    if ( section == 0 ) {
      return "Label";
    }
    else if ( section == 1 ) {
      return "Value";
    }
  }
  return QVariant();
}

// @brief 関連するパース木をセットする．
void
VlParseTreeModel::set_pt(const VlMgr& vl_mgr)
{
  delete mRootNode;
  mRootNode = new RootNode(vl_mgr);
}
  
// @brief トークンのファイル上の位置を返す．
FileRegion
VlParseTreeModel::loc(const QModelIndex& index)
{
  VlPtNode* node = index2node(index);
  if ( node ) {
    return node->loc();
  }
  else {
    return FileRegion();
  }
}

// @brief インデックスをノードに変換する．
VlPtNode*
VlParseTreeModel::index2node(const QModelIndex& index) const
{
  if ( index.isValid() ) {
    return static_cast<VlPtNode*>(index.internalPointer());
  }
  else {
    return mRootNode;
  }
}

END_NAMESPACE_YM_VERILOG
