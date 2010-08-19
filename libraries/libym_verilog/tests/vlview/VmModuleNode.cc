
/// @file libym_verilog/tests/vlview/VmModuleNode.cc
/// @brief VmModuleNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmModuleNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmModuleNode.h"
#include "VmDeclNode.h"
//#include "VlPtNode_item.h"
#include "VmExprNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmModuleNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module Module の構造を表すオブジェクト
VmModuleNode::VmModuleNode(const VlModule* module) :
  mModule(module)
{
}

// @brief デストラクタ
VmModuleNode::~VmModuleNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiModule";
    }
    else if ( column == 1 ) {
      return mModule->name();
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleNode::loc() const
{
  return mModule->file_region();
}

BEGIN_NONAMESPACE

// 時間の単位を表す文字列を得る．
QString
unit2str(int unit)
{
  if ( unit > 2 || unit < -15 ) {
    return "illegal time unit";
  }

  unit += 15;
  QString ans;
  switch ( unit % 3 ) {
  case 0: ans = "1"; break;
  case 1: ans = "10"; break;
  case 2: ans = "100"; break;
  }
  switch ( unit / 3 ) {
  case 5: ans += "s"; break;
  case 4: ans += "ms"; break;
  case 3: ans += "us"; break;
  case 2: ans += "ns"; break;
  case 1: ans += "ps"; break;
  case 0: ans += "fs"; break;
  }
  return ans;
}

END_NONAMESPACE

// @brief 子供の配列を作る．
void
VmModuleNode::expand() const
{
  add_child("vpiDefName", mModule->def_name());
  add_child("vpiFullName", mModule->full_name());
  // vpiDefFile

  const VlModuleArray* module_array = mModule->module_array();
  bool is_array = (module_array != NULL);
  add_child( new VmBoolNode("vpiArray", is_array) );
  if ( is_array ) {
    add_child("vpiModuleArray", module_array->full_name());
    add_child( new VmIntNode("vpiIndex", mModule->index()) );
  }
  add_child("vpiTopModule", mModule->is_top_module());
  add_child("vpiCellInstance", mModule->is_cell_instance());
  add_child("vpiProtected", mModule->is_protected());
  if ( mModule->time_unit() != -16 ) {
    add_child("vpiTimeUnit", unit2str(mModule->time_unit()));
    add_child("vpiTimePrecision", unit2str(mModule->time_precision()));
  }
  add_child( new VmNetTypeNode("vpiDefaultNetType", mModule->def_net_type()) );
  add_child( new VmUdNode(mModule->unconn_drive()) );
  add_child( new VmDelayModeNode(mModule->def_delay_mode()) );
  add_child("vpiDefaultDecayTime", mModule->def_decay_time());
  
  add_child("Config", mModule->config().c_str());
  add_child("Library", mModule->library().c_str());
  add_child("Cell", mModule->cell().c_str());

  add_child( new VmPortListNode(mModule) );
  ymuint n = mModule->io_num();
  vector<const VlIODecl*> io_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    io_array[i] = mModule->io(i);
  }
  add_child( new VmIODeclListNode(io_array) );

#if 0
  put_scope_sub(mgr, module);
  
  vector<const VlProcess*> process_list;
  if ( mgr.find_process_list(mModule, process_list) ) {
    put_process_list("vpiProcess", mgr, process_list);
  }
  if ( mModule->paramport_array().size() > 0 ) {
    add_child( new DeclHeadListNode("Parameter Port",
					      mModule->paramport_array()) );
  }
  if ( mModule->port_num() > 0 ) {
    add_child( new ModulePortListNode(mModule) );
  }
  if ( mModule->iohead_array().size() > 0 ) {
    add_child( new IOHeadListNode(mModule->iohead_array()) );
  }
  if ( mModule->paramhead_array().size() > 0 ) {
    add_child( new DeclHeadListNode("Parameter",
					      mModule->paramhead_array()) );
  }
  if ( mModule->localparamhead_array().size() > 0 ) {
    add_child( new DeclHeadListNode("Localparam",
					      mModule->localparamhead_array()) );
  }
  if ( mModule->declhead_array().size() > 0 ) {
    add_child( new DeclHeadListNode("Decl",
					      mModule->declhead_array()) );
  }
  if ( mModule->item_array().size() > 0 ) {
    add_child( new ItemListNode("Item",
					  mModule->item_array()) );
  }
#endif
}


//////////////////////////////////////////////////////////////////////
// クラス VmPortListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] module Module を表すパース木
VmPortListNode::VmPortListNode(const VlModule* module) :
  mModule(module)
{
}

// @brief デストラクタ
VmPortListNode::~VmPortListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPortListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPortList";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPortListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmPortListNode::expand() const
{
  for (ymuint i = 0; i < mModule->port_num(); ++ i) {
    add_child( new VmPortNode(mModule->port(i)) );
  }
}

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
VmPortNode::loc() const
{
  return mPort->file_region();
}

// @brief 子供の配列を作る．
void
VmPortNode::expand() const
{
  add_child("vpiModule", mPort->module()->full_name());
  add_child("vpiConnByName", mPort->is_conn_by_name());
  add_child("vpiExplicitName", mPort->is_explicit_name());
  add_child("vpiPortIndex", mPort->port_index());
#if 0
  add_child("vpiScalar", mPort->is_scalar());
  add_child("vpiVector", mPort->is_vector());
#endif
  add_child("vpiSzie", mPort->bit_size());
  add_child( new VmExprNode("vpiHighConn", mPort->high_conn()) );
  add_child( new VmExprNode("vpiLowConn", mPort->low_conn()) );
}

END_NAMESPACE_YM_VERILOG
