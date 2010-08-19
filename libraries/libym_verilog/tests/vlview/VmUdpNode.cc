
/// @file libym_verilog/tests/vlview/VmUdpNode.cc
/// @brief VmUdpNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_udp.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmUdpNode.h"
#include "VmDeclNode.h"
#include "VmExprNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlUdp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmUdpNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] udp UDP を表すオブジェクト
VmUdpNode::VmUdpNode(const VlUdpDefn* udp) :
  mUdp(udp)
{
}

// @brief デストラクタ
VmUdpNode::~VmUdpNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP";
    }
    else if ( column == 1 ) {
      return mUdp->def_name();
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpNode::loc() const
{
  return mUdp->file_region();
}

// @brief 子供の配列を作る．
void
VmUdpNode::expand() const
{
  add_child( new VmPrimTypeNode(mUdp->prim_type()) );
  add_child("vpiProtected", mUdp->is_protected());
  ymuint n = mUdp->port_num();
  vector<const VlIODecl*> ioarray(n);
  for (ymuint i = 0; i < n - 1; ++ i) {
    ioarray[i] = mUdp->input(i);
  }
  ioarray[n - 1] = mUdp->output();
  add_child( new VmIODeclListNode(ioarray) );
  if ( mUdp->init_expr() ) {
    add_child("vpiInitial", mUdp->init_expr());
  }
  add_child( new VmUdpEntryListNode(mUdp) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdpEntryListNode
//////////////////////////////////////////////////////////////////////

/// @brief コンストラクタ
/// @param[in] udp UDP を表すオブジェクト
VmUdpEntryListNode::VmUdpEntryListNode(const VlUdpDefn* udp) :
  mUdp(udp)
{
}

// @brief デストラクタ
VmUdpEntryListNode::~VmUdpEntryListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpEntryListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP Table Entry List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpEntryListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmUdpEntryListNode::expand() const
{
  ymuint n = mUdp->table_size();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmUdpEntryNode(mUdp->table_entry(i)) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdpEntryNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] table テーブルの1行分
VmUdpEntryNode::VmUdpEntryNode(const VlTableEntry* table) :
  mTable(table)
{
}

// @brief デストラクタ
VmUdpEntryNode::~VmUdpEntryNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdpEntryNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "UDP Table Entry";
    }
    else if ( column == 1 ) {
      return mTable->str().c_str();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmUdpEntryNode::loc() const
{
  return mTable->file_region();
}

// @brief 子供の配列を作る．
void
VmUdpEntryNode::expand() const
{
}

END_NAMESPACE_YM_VERILOG
