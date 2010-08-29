
/// @file libym_verilog/tests/vlview/VmModuleNode.cc
/// @brief VmModuleNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmModuleNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmModuleNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/VlMgr.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlPort.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmNode の関数
//////////////////////////////////////////////////////////////////////

// @brief ModuleNode を追加する．
// @param[in] label ラベル
// @param[in] module モジュール
void
VmNode::add_child(const QString& label,
		  const VlModule* module) const
{
  add_child( new VmModuleNode(vl_mgr(), label, module) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
VmModuleListNode::VmModuleListNode(const VlMgr& vl_mgr,
				   const QString& label) :
  VmNode(vl_mgr),
  mLabel(label)
{
}

// @brief デストラクタ
VmModuleListNode::~VmModuleListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmModuleListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmModuleListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmModuleListNode::expand() const
{
  const list<const VlModule*>& module_list = vl_mgr().topmodule_list();
  for (list<const VlModule*>::const_iterator p = module_list.begin();
       p != module_list.end(); ++ p) {
    add_child("vpiModule", *p);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmModuleNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] module Module の構造を表すオブジェクト
VmModuleNode::VmModuleNode(const VlMgr& vl_mgr,
			   const QString& label,
			   const VlModule* module) :
  VmNode(vl_mgr),
  mLabel(label),
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
      return mLabel;
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
  add_child("vpiArray", is_array);
  if ( is_array ) {
    add_child("vpiModuleArray", module_array->full_name());
    add_child("vpiIndex", mModule->index());
  }
  add_child("vpiTopModule", mModule->is_top_module());
  add_child("vpiCellInstance", mModule->is_cell_instance());
  add_child("vpiProtected", mModule->is_protected());
  if ( mModule->time_unit() != -16 ) {
    add_child("vpiTimeUnit", unit2str(mModule->time_unit()));
    add_child("vpiTimePrecision", unit2str(mModule->time_precision()));
  }
  add_child( new VmNetTypeNode(vl_mgr(), "vpiDefaultNetType", mModule->def_net_type()) );
  add_child( new VmUdNode(vl_mgr(), mModule->unconn_drive()) );
  add_child( new VmDelayModeNode(vl_mgr(), mModule->def_delay_mode()) );
  add_child("vpiDefaultDecayTime", mModule->def_decay_time());
  
  add_child("Config", mModule->config().c_str());
  add_child("Library", mModule->library().c_str());
  add_child("Cell", mModule->cell().c_str());

  add_child( new VmPortListNode(vl_mgr(), "vpiPort", mModule) );
  ymuint n = mModule->io_num();
  vector<const VlIODecl*> io_array(n);
  for (ymuint i = 0; i < n; ++ i) {
    io_array[i] = mModule->io(i);
  }
  add_child("vpiIO", io_array);

#if 0
  put_scope_sub(mgr, module);
#endif
  
  vector<const VlProcess*> process_list;
  if ( vl_mgr().find_process_list(mModule, process_list) ) {
    add_child("vpiProcess", process_list);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPortListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] module Module を表すパース木
VmPortListNode::VmPortListNode(const VlMgr& vl_mgr,
			       const QString& label,
			       const VlModule* module) :
  VmNode(vl_mgr),
  mLabel(label),
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
      return mLabel;
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
    const VlPort* port = mModule->port(i);
    add_child( new VmPortNode(vl_mgr(), mLabel, port) );
  }
}

//////////////////////////////////////////////////////////////////////
// クラス VlPortNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vl_mgr VlMgr
// @param[in] label ラベル
// @param[in] port ポートを表すオブジェクト
VmPortNode::VmPortNode(const VlMgr& vl_mgr,
		       const QString& label,
		       const VlPort* port) :
  VmNode(vl_mgr),
  mLabel(label),
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
      return mLabel;
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
  if ( mPort->high_conn() ) {
    add_child("vpiHighConn", mPort->high_conn());
  }
  if ( mPort->low_conn() ) {
    add_child("vpiLowConn", mPort->low_conn());
  }
}

END_NAMESPACE_YM_VERILOG
