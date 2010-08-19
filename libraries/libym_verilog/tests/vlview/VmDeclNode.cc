
/// @file libym_verilog/tests/vlview/VmDeclNode.cc
/// @brief VlDeclNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmDeclNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2009 Yusuke Matsunaga
/// All rights reserved.


#include "VmDeclNode.h"
#include "VmExprNode.h"
#include "VmMiscNode.h"
#include "ym_verilog/vl/VlIODecl.h"
#include "ym_verilog/vl/VlDecl.h"


BEGIN_NAMESPACE_YM_VERILOG

#if 0
//////////////////////////////////////////////////////////////////////
// クラス VmIODeclListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] iohead IO宣言ヘッダ
VmIODeclListNode::VmIODeclListNode(const PtIOHead* iohead) :
  mIOHead(iohead)
{
}

// @brief デストラクタ
IOItemListNode::~IOItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
IOItemListNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "IO Item List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
IOItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
IOItemListNode::expand() const
{
  ymuint32 n = mIOHead->item_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new IOItemNode(mIOHead->item(i));
  }
}
#endif

//////////////////////////////////////////////////////////////////////
// クラス VmIODeclNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
VlIODeclNode::VlIODeclNode(const VlIODecl* io) :
  mIODecl(io)
{
}

// @brief デストラクタ
VlIODeclNode::~VlIODeclNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VlIODeclNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "IO decl";
    }
    else if ( column == 1 ) {
      return mIODecl->name();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VlIODeclNode::loc() const
{
  return mIODecl->file_region();
}

// @brief 子供の配列を作る．
void
VlIODeclNode::expand() const
{
  add_child( new VmDirectionNode(mIODecl->direction()) );
  add_child("vpiSigned", mIODecl->is_signed());
  add_child("vpiScalar", mIODecl->is_scalar());
  add_child("vpiVector", mIODecl->is_vector());
  add_child("vpiSize", mIODecl->bit_size());

  const VlModule* module = mIODecl->module();
  if ( module ) {
    add_child("vpiModule", module->full_name());
  }
  const VlUdpDefn* udp = mIODecl->udp_defn();
  if ( udp ) {
    add_child("vpiUdpDefn", udp->def_name());
  }
  const VlTask* task = mIODecl->task();
  if ( task ) {
    add_child("vpiTask", task->full_name());
  }
  const VlFunction* func = mIODecl->function();
  if ( func ) {
    add_child("vpiFunction", func->full_name());
  }
  add_child("vpiLeftRange", mIODecl->left_range());
  add_child("vpiRightRange", mIODecl->right_range());
}


//////////////////////////////////////////////////////////////////////
// クラス DeclHeadListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] declhead_array 宣言ヘッダの配列
DeclHeadListNode::DeclHeadListNode(const QString& label,
				   const PtDeclHeadArray& declhead_array) :
  mLabel(label),
  mDeclHeadArray(declhead_array)
{
}

// @brief デストラクタ
DeclHeadListNode::~DeclHeadListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DeclHeadListNode::data(int column,
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
DeclHeadListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
DeclHeadListNode::expand() const
{
   ymuint32 n = mDeclHeadArray.size();
   mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new DeclHeadNode(mDeclHeadArray[i]);
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DeclHeadNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] declhead 宣言ヘッダ
DeclHeadNode::DeclHeadNode(const PtDeclHead* declhead) :
  mDeclHead(declhead)
{
}

// @brief デストラクタ
DeclHeadNode::~DeclHeadNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DeclHeadNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      switch ( mDeclHead->type() ) {
      case kPtDecl_Param:
	switch ( mDeclHead->data_type() ) {
	case kVpiVarNone:        return "Parameter";
	case kVpiVarInteger:     return "Parameter(integer)"; 
	case kVpiVarReal:        return "Parameter(real)";
	case kVpiVarTime:        return "Parameter(time)";
	case kVpiVarRealtime:    return "Parameter(realtime)";
	}
	break;
	
      case kPtDecl_LocalParam:
	switch ( mDeclHead->data_type() ) {
	case kVpiVarNone:        return "Localparam";
	case kVpiVarInteger:     return "Localparam(integer)"; 
	case kVpiVarReal:        return "Localparam(real)";
	case kVpiVarTime:        return "Localparam(time)";
	case kVpiVarRealtime:    return "Localparam(realtime)";
	}
	break;
	
      case kPtDecl_Reg:          return "Reg";
      case kPtDecl_Var:
	switch ( mDeclHead->data_type() ) {
	case kVpiVarInteger:     return "Integer";
	case kVpiVarReal:        return "Real";   
	case kVpiVarTime:        return "Time";
	case kVpiVarRealtime:    return "Realtime";
	case kVpiVarNone:
	  assert_not_reached(__FILE__, __LINE__);
	}
	break;
	
      case kPtDecl_Genvar:       return "Genvar";    
      case kPtDecl_Event:        return "Event";     
      case kPtDecl_SpecParam:    return "Specparam"; 
      case kPtDecl_Net:
	switch ( mDeclHead->net_type() ) {
	case kVpiSupply0:        return "Supply0";
	case kVpiSupply1:        return "Supply1";
	case kVpiTri:            return "Tri";
	case kVpiTriAnd:         return "Triand";
	case kVpiTriOr:          return "Trior";
	case kVpiTriReg:         return "Trireg";
	case kVpiTri0:           return "Tri0";
	case kVpiTri1:           return "Tri1";
	case kVpiWire:           return "Wire";
	case kVpiWand:           return "Wand";
	case kVpiWor:            return "Wor";
	case kVpiNone:           return "None";
	}
	break;
      }
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DeclHeadNode::loc() const
{
  return mDeclHead->file_region();
}

// @brief 子供の配列を作る．
void
DeclHeadNode::expand() const
{
  mChildren.push_back( new BoolNode("Signed", mDeclHead->is_signed()) );
  if ( mDeclHead->left_range() ) {
    mChildren.push_back( new ExprNode("Left Range", mDeclHead->left_range()) );
    mChildren.push_back( new ExprNode("Right Range", mDeclHead->right_range()) );
  }
  if ( mDeclHead->vs_type() != kVpiVsNone ) {
    mChildren.push_back( new VsNode(mDeclHead->vs_type()) );
  }
  if ( mDeclHead->strength() ) {
    mChildren.push_back( new StrengthNode(mDeclHead->strength()) );
  }
  if ( mDeclHead->delay() ) {
    mChildren.push_back( new DelayNode( mDeclHead->delay() ) );
  }
  mChildren.push_back( new DeclItemListNode(mDeclHead) );
}


//////////////////////////////////////////////////////////////////////
// クラス DeclItemListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] declhead IO宣言ヘッダ
DeclItemListNode::DeclItemListNode(const PtDeclHead* declhead) :
  mDeclHead(declhead)
{
}

// @brief デストラクタ
DeclItemListNode::~DeclItemListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DeclItemListNode::data(int column,
		       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Decl Item List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DeclItemListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
DeclItemListNode::expand() const
{
  ymuint32 n = mDeclHead->item_num();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new DeclItemNode(mDeclHead->item(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmDeclNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 式
VmDeclNode::VmDeclNode(const VlDecl* decl) :
  mDecl(decl)
{
}

// @brief デストラクタ
VmDeclNode::~VmDeclNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDeclNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Decl";
    }
    else if ( column == 1 ) {
      return mDecl->name();
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
VmDeclNode::loc() const
{
  return mDecl->file_region();
}

// @brief 子供の配列を作る．
void
VmDeclNode::expand() const
{
  const char* nm = NULL;
  switch ( mDecl->type() ) {
  case kVpiNet:             nm = "Net"; break;
  case kVpiNetArray:        nm = "NetArray"; break;
  case kVpiReg:             nm = "Reg"; break;
  case kVpiRegArray:        nm = "RegArray"; break;
  case kVpiIntegerVar:      nm = "IntegerVar"; break;
  case kVpiRealVar:         nm = "RealVar"; break;
  case kVpiTimeVar:         nm = "TimeVar"; break;
  case kVpiVarSelect:       nm = "VarSelect"; break;
  case kVpiNamedEvent:      nm = "NamedEvent"; break;
  case kVpiNamedEventArray: nm = "NamedEventArray"; break;
  case kVpiParameter:       nm = "Parameter"; break;
  case kVpiSpecParam:       nm = "SpecParam"; break;
  default: assert_not_reached( __FILE__, __LINE__ );
  }
  add_child("vpiType", nm);
  add_child("vpiFullName", mDecl->full_name());
  add_child("vpiExpanded", mDecl->is_expanded());
  add_child("vpiImplicitDecl", mDecl->is_implicit_decl());
  add_child("vpiNetDeclAssign", mDecl->has_net_decl_assign());
  add_child("vpiScalar", mDecl->is_scalar());
  add_child("vpiVector", mDecl->is_vector());
  add_child("vpiExplicitScalar", mDecl->is_explicit_scalar());
  add_child("vpiExplicitVector", mDecl->is_explicit_vector());
  add_child("vpiSigned", mDecl->is_signed());
  add_child("vpiSize", mDecl->bit_size());
  add_child("vpiModule", mDecl->parent_module()->full_name());
  add_child("vpiScope", mDecl->parent()->full_name());
  add_child("vpiArray", mDecl->is_array());
  add_child("vpiMultiArray", mDecl->is_multi_array());
  // 親の配列

  ymuint n = mDecl->dimension();
  for (ymuint i = 0; i < n; ++ i) {
    add_child( new VmRangeNode("vpiRange", mDecl->range(i)) );
  }

  if ( mDecl->type() == kVpiNet ) {
    add_child( new VmStrengthValNode("vpiStrength0", mDecl->drive0()) );
    add_child( new VmStrengthValNode("vpiStrength1", mDecl->drive1()) );
    add_child( new VmStrengthValNode("vpiChargeStrength", mDecl->charge()) );
    add_child( new VmDelayNode("vpiDelay", mDecl->delay()) );
  }

  add_child( new VmExprNode("vpiIndex", mDecl->index()) );

  if ( mDecl->port_inst() ) {
    add_child( new VmExprNode("vpiPortInst", mDecl->port_inst()) );
  }
  if ( mDecl->ports() ) {
    add_child( new VmExprNode("vpiPorts", mDecl->ports()) );
  }

  if ( mDecl->left_range() ) {
    add_child( new VmExprNode("vpiLeftRange", mDecl->left_range()) );
    add_child( new VmExprNode("vpiRightRange", mDecl->right_range()) );
  }
}


//////////////////////////////////////////////////////////////////////
// クラス RangeListNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] declitem 親の宣言要素
RangeListNode::RangeListNode(const PtDeclItem* declitem) :
  mDeclItem(declitem)
{
}

// @brief デストラクタ
RangeListNode::~RangeListNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
RangeListNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Range List";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
RangeListNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
void
RangeListNode::expand() const
{
  ymuint32 n = mDeclItem->dimension_list_size();
  mChildren.resize(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    mChildren[i] = new RangeNode(mDeclItem->range(i));
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmRangeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] range 範囲
VmRangeNode::VmRangeNode(const VlRange* range) :
  mRange(range)
{
}

// @brief デストラクタ
VmRangeNode::~VmRangeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmRangeNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Range";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
RangeNode::loc() const
{
  return mRange->file_region();
}

// @brief 子供の配列を作る．
void
RangeNode::expand() const
{
  add_child( new VmExprNode("vpiLeftRange", mRange->left()) );
  add_child( new VmExprNode("vpiRightRange", mRange->right()) );
}

END_NAMESPACE_YM_VERILOG
