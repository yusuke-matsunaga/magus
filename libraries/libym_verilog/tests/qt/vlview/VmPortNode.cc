
/// @file VmPortNode.cc
/// @brief VmPortNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2009, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "VmPortNode.h"
#include "VmExprNode.h"
#include "VmMiscNode.h"
#include "YmVerilog/vl/VlPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VlPortNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] port ポートを表すオブジェクト
VmPortNode::VmPortNode(const VlPort* port) :
  mPort(port)
{
}

// @brief デストラクタ
VmPortNode::~VmPortNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role
QVariant
VmPortNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPort";
    }
    else if ( column == 1 ) {
      if ( mPort->name() ) {
	return mPort->name();
      }
      else {
	return "";
      }
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
PortNode::loc() const
{
  return mPort->file_region();
}

// @brief 子供の配列を作る．
void
PortNode::expand() const
{
  add_child("vpiModule", mPort->module()->full_name());
  add_child("vpiConnByName", mPort->is_conn_by_name());
  add_child("vpiExplicitName", mPort->is_explicit_name());
  add_child("vpiPortIndex", mPort->port_index());
  add_child("vpiScalar", mPort->is_scalar());
  add_child("vpiVector", mPort->is_vector());
  add_child("vpiSzie", mPort->bit_size());
  add_child( new VmExprNode("vpiHighConn", mPort->high_conn()) );
  add_child( new VmExprNode("vpiLowConn", mPort->low_conn()) );
}

END_NAMESPACE_YM_VERILOG
