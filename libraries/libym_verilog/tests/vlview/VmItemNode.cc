
/// @file libym_verilog/tests/vlview/VmItermNode.cc
/// @brief VmItemNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VlPtNode_item.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmItemNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlPrimitive.h"
#include "ym_verilog/vl/VlContAssign.h"
#include "ym_verilog/vl/VlModule.h"
#include "ym_verilog/vl/VlUdp.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス VmPrimitiveListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] prim_list プリミティブのリスト
VmPrimitiveListNode::VmPrimitiveListNode(const vector<const VlPrimitive*>& prim_list) :
  mPrimArray(prim_list)
{
}

// @brief デストラクタ
VmPrimitiveListNode::~VmPrimitiveListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimitiveListNode::data(int column,
			  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPtimitive";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimitiveListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmPrimitiveListNode::expand() const
{
  for (vector<const VlPrimitive*>::const_iterator p = mPrimArray.begin();
       p != mPrimArray.end(); ++ p) {
    add_child( new VmPrimitiveNode(*p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimitiveNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] primitive プリミティブ
VmPrimitiveNode::VmPrimitiveNode(const VlPrimitive* primitive) :
  mPrimitive(primitive)
{
}

// @brief デストラクタ
VmPrimitiveNode::~VmPrimitiveNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimitiveNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPtimitive";
    }
    else if ( column == 1 ) {
      const char* nm = NULL;
      switch ( mPrimitive->type() ) {
      case kVpiGate:   nm = "vpiGate"; break;
      case kVpiSwitch: nm = "vpiSwitch"; break;
      case kVpiUdp:    nm = "vpiUdp"; break;
      default: assert_not_reached(__FILE__, __LINE__);
      }
      return nm;
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimitiveNode::loc() const
{
  return mPrimitive->file_region();
}

// @brief 子供の配列を作る．
void
VmPrimitiveNode::expand() const
{
  add_str("vpiFullName", mPrimitive->full_name());
  add_str("vpiDefName", mPrimitive->def_name());
  add_child( new VmPrimTypeNode(mPrimitive->prim_type()) );

  add_str("vpiModule", mPrimitive->parent_module()->full_name());
  add_str("vpiScope", mPrimitive->parent()->full_name());

  add_strength("vpiStrength0", mPrimitive->drive0());
  add_strength("vpiStrength1", mPrimitive->drive1());
  add_delay(mPrimitive->delay());

  if ( mPrimitive->type() == kVpiUdp ) {
    add_str("vpiUdpDefn", mPrimitive->udp_defn()->def_name());
  }
  add_int("port num", mPrimitive->port_num());

  add_child( new VmPrimTermListNode(mPrimitive) );
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimTermListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] primitive 親のプリミティブ
VmPrimTermListNode::VmPrimTermListNode(const VlPrimitive* primitive) :
  mPrimitive(primitive)
{
}

// @brief デストラクタ
VmPrimTermListNode::~VmPrimTermListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimTermListNode::data(int column,
			 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPtimTerm";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimTermListNode::loc() const
{
  ymuint n = mPrimitive->port_num();
  if ( n == 0 ) {
    return FileRegion();
  }
  else {
    FileRegion first = mPrimitive->prim_term(0)->file_region();
    FileRegion last = mPrimitive->prim_term(n - 1)->file_region();
    return FileRegion(first, last);
  }
}

// @brief 子供の配列を作る．
void
VmPrimTermListNode::expand() const
{
  ymuint n = mPrimitive->port_num();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmPrimTermNode(mPrimitive->prim_term(i)) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimTermNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] prim_term プリミティブの端子
VmPrimTermNode::VmPrimTermNode(const VlPrimTerm* prim_term) :
  mPrimTerm(prim_term)
{
}

// @brief デストラクタ
VmPrimTermNode::~VmPrimTermNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimTermNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPtimTerm";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmPrimTermNode::loc() const
{
  return mPrimTerm->file_region();
}

// @brief 子供の配列を作る．
void
VmPrimTermNode::expand() const
{
  add_str("vpiPrimitive", mPrimTerm->primitive()->full_name());
  add_int("vpiTermIndex", mPrimTerm->term_index());
  add_dir(mPrimTerm->direction());
  add_expr("vpiExpr", mPrimTerm->expr());
}


//////////////////////////////////////////////////////////////////////
// クラス VmContAssignListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ca_list 継続的代入文のリスト
VmContAssignListNode::VmContAssignListNode(const vector<const VlContAssign*>& ca_list) :
  mContAssignArray(ca_list)
{
}

// @brief デストラクタ
VmContAssignListNode::~VmContAssignListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmContAssignListNode::data(int column,
			   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiContAssign";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmContAssignListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
VmContAssignListNode::expand() const
{
  for (vector<const VlContAssign*>::const_iterator p = mContAssignArray.begin();
       p != mContAssignArray.end(); ++ p) {
    add_child( new VmContAssignNode(*p) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmContAssignNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] cont_assign 継続的代入文
VmContAssignNode::VmContAssignNode(const VlContAssign* cont_assign) :
  mContAssign(cont_assign)
{
}

// @brief デストラクタ
VmContAssignNode::~VmContAssignNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmContAssignNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiContAssign";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}

// @brief 対象のファイル上での位置を返す．
FileRegion
VmContAssignNode::loc() const
{
  return mContAssign->file_region();
}

// @brief 子供の配列を作る．
void
VmContAssignNode::expand() const
{
  add_bool("vpiNetDeclAssign", mContAssign->has_net_decl_assign());
  add_str("vpiModule", mContAssign->module()->full_name());

  add_strength("vpiStrength0", mContAssign->drive0());
  add_strength("vpiStrength1", mContAssign->drive1());
  add_delay(mContAssign->delay());

  add_int("vpiSize", mContAssign->bit_size());
}

END_NAMESPACE_YM_VERILOG
