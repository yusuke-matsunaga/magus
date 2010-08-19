
/// @file libym_verilog/tests/vlview/VmMiscNode.cc
/// @brief VmMiscNode の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: VmMiscNode.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "VmMiscNode.h"


BEGIN_NAMESPACE_YM_VERILOG

#if 0
//////////////////////////////////////////////////////////////////////
// クラス ControlNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] control コントロール
ControlNode::ControlNode(const PtControl* control) :
  mControl(control)
{
}

// @brief デストラクタ
ControlNode::~ControlNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
ControlNode::data(int column,
		  int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      switch ( mControl->type() ) {
      case kPtDelayControl:  return "DelayControl";
      case kPtEventControl:  return "EventControl";
      case kPtRepeatControl: return "RepeatControl";
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
ControlNode::loc() const
{
  return mControl->file_region();
}

// @brief 子供の配列を作る．
void
ControlNode::expand() const
{
  switch ( mControl->type() ) {
  case kPtDelayControl:
    mChildren.reserve(1);
    mChildren.push_back( new ExprNode("Delay", mControl->delay()) );
    break;
    
  case kPtEventControl:
    {
      ymuint32 n = mControl->event_num();
      mChildren.reserve(n);
      for (ymuint32 i = 0; i < n; ++ i) {
	mChildren.push_back( new ExprNode("Event", mControl->event(i)) );
      }
    }
    break;
    
  case kPtRepeatControl:
    {
      ymuint32 n = mControl->event_num();
      mChildren.reserve(n + 1);
      mChildren.push_back( new ExprNode("Repeat", mControl->rep_expr()) );
      for (ymuint32 i = 0; i < n; ++ i) {
	mChildren.push_back( new ExprNode("Event", mControl->event(i)) );
      }
    }
    break;
  }
}


//////////////////////////////////////////////////////////////////////
// クラス DelayNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] con 接続
DelayNode::DelayNode(const PtDelay* delay) :
  mDelay(delay)
{
}

// @brief デストラクタ
DelayNode::~DelayNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
DelayNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "Delay";
    }
    else if ( column == 1 ) {
      return "";
    }
  }
  return QVariant();
}
    
// @brief 対象のファイル上での位置を返す．
FileRegion
DelayNode::loc() const
{
  return mDelay->file_region();
}

// @brief 子供の配列を作る．
void
DelayNode::expand() const
{
  ymuint32 n = 0;
  for (ymuint32 i = 0; i < 3; ++ i) {
    if ( mDelay->value(i) ) {
      ++ n;
    }
  }
  mChildren.reserve(n);
  for (ymuint32 i = 0; i < 3; ++ i) {
    if ( mDelay->value(i) ) {
      QString label("Delay");
      label += static_cast<char>('0' + i);
      mChildren.push_back( new ExprNode(label, mDelay->value(i)) );
    }
  }
}


//////////////////////////////////////////////////////////////////////
// クラス VmScalarNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
VmScalarNode::VmScalarNode()
{
}

// @brief デストラクタ
VmScalarNode::~VmScalarNode()
{
}

// @brief 対象のファイル上での位置を返す．
// @note このクラスでは空の FileRegion を返す．
FileRegion
VmScalarNode::loc() const
{
  return FileRegion();
}

// @brief 子供の配列を作る．
// @note このクラスではなにもしない．
void
VmScalarNode::expand() const
{
}


//////////////////////////////////////////////////////////////////////
// クラス VmStrNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] str 文字列
VmStrNode::VmStrNode(const QString& label,
		     const QString& str) :
  mLabel(label),
  mStr(str)
{
}

// @brief デストラクタ
VmStrNode::~VmStrNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmStrNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return mStr;
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmAuxTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] aux_type 補助的なデータ型
// @param[in] net_type ネットの型
// @param[in] var_type 変数の型
VmAuxTypeNode::VmAuxTypeNode(tVpiAuxType aux_type,
			     tVpiNetType net_type,
			     tVpiVarType var_type) :
  mAuxType(aux_type),
  mNetType(net_type),
  mVarType(var_type)
{
}

// @brief デストラクタ
VmAuxTypeNode::~VmAuxTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmAuxTypeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiAuxType";
    }
    else if ( column == 1 ) {
      switch ( mAuxType ) {
      case kVpiAuxNone: return "None";
      case kVpiAuxNet:
	switch ( mNetType ) {
	case kVpiSupply0: return "Supply0";
	case kVpiSupply1: return "Supply1";
	case kVpiTri:     return "Tri";
	case kVpiTriAnd:  return "Triand";
	case kVpiTriOr:   return "Trior";
	case kVpiTriReg:  return "Trireg";
	case kVpiTri0:    return "Tri0";
	case kVpiTri1:    return "Tri1";
	case kVpiWire:    return "Wire";
	case kVpiWand:    return "Wand";
	case kVpiWor:     return "Wor";
	case kVpiNone:    return "None";
	}
      case kVpiAuxReg:  return "Reg";
      case kVpiAuxVar:
	switch ( mVarType ) {
	case kVpiVarInteger:  return "Integer";
	case kVpiVarReal:     return "Real";
	case kVpiVarTime:     return "Time";
	case kVpiVarRealtime: return "Realtime";
	case kVpiVarNone:     return "None";
	}
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmNetTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] net_type ネットの型
VmNetTypeNode::VmNetTypeNode(const QString& label,
			     tVpiNetType net_type) :
  mLabel(label),
  mNetType(net_type)
{
}

// @brief デストラクタ
VmNetTypeNode::~VmNetTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmNetTypeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      switch ( mNetType ) {
      case kVpiSupply0: return "Supply0";
      case kVpiSupply1: return "Supply1";
      case kVpiTri:     return "Tri";
      case kVpiTriAnd:  return "Triand";
      case kVpiTriOr:   return "Trior";
      case kVpiTriReg:  return "Trireg";
      case kVpiTri0:    return "Tri0";
      case kVpiTri1:    return "Tri1";
      case kVpiWire:    return "Wire";
      case kVpiWand:    return "Wand";
      case kVpiWor:     return "Wor";
      case kVpiNone:    return "None";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmVarTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var_type 変数の型
VmVarTypeNode::VmVarTypeNode(tVpiVarType var_type) :
  mVarType(var_type)
{
}

// @brief デストラクタ
VmVarTypeNode::~VmVarTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmVarTypeNode::data(int column,
		    int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiVarType";
    }
    else if ( column == 1 ) {
      switch ( mVarType ) {
      case kVpiVarInteger:  return "Integer";
      case kVpiVarReal:     return "Real";
      case kVpiVarTime:     return "Time";
      case kVpiVarRealtime: return "Realtime";
      case kVpiVarNone:     return "None";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmDirNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] dir 入出力の方向
VmDirNode::VmDirNode(tVpiDirection dir) :
  mDir(dir)
{
}

// @brief デストラクタ
VmDirNode::~VmDirNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDirNode::data(int column,
		int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiDirection";
    }
    else if ( column == 1 ) {
      switch ( mDir ) {
      case kVpiInput:       return "Input";
      case kVpiOutput:      return "Output";
      case kVpiInout:       return "Inout";
      case kVpiMixedIO:     return "Mixed IO";
      case kVpiNoDirection: return "No Direction";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmUdNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] ud unconnected drive の型
VmUdNode::VmUdNode(tVpiUnconnDrive ud) :
  mUd(ud)
{
}

// @brief デストラクタ
VmUdNode::~VmUdNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmUdNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiUnconnectedDrive";
    }
    else if ( column == 1 ) {
      switch ( mUd ) {
      case kVpiHighZ: return "high-Z";
      case kVpiPull1: return "pull-1";
      case kVpiPull0: return "pull-0";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmDelayModeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] delay mode の型
VmDelayModeNode::VmDelayModeNode(tVpiDefDelayMode delay_mode) :
  mDelayMode(delay_mode)
{
}

// @brief デストラクタ
VmDelayModeNode::~VmDelayModeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmDelayModeNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiDefaultDelayMode";
    }
    else if ( column == 1 ) {
      switch ( mDelayMode ) {
      case kVpiDelayModeNone:    return "none";
      case kVpiDelayModePath:    return "path";
      case kVpiDelayModeDistrib: return "distrib";
      case kVpiDelayModeUnit:    return "unit";
      case kVpiDelayModeZero:    return "zero";
      case kVpiDelayModeMTM:     return "min-typ-max";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmPrimTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] prim_type primitive タイプ
VmPrimTypeNode::VmPrimTypeNode(tVpiPrimType prim_type) :
  mPrimType(prim_type)
{
}

// @brief デストラクタ
VmPrimTypeNode::~VmPrimTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmPrimTypeNode::data(int column,
		     int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiPrimType";
    }
    else if ( column == 1 ) {
      switch ( mPrimType ) {
      case kVpiAndPrim:      return "and";
      case kVpiNandPrim:     return "nand";
      case kVpiNorPrim:      return "nor";
      case kVpiOrPrim:       return "or";
      case kVpiXorPrim:      return "xor";
      case kVpiXnorPrim:     return "xnor";
      case kVpiBufPrim:      return "buf";
      case kVpiNotPrim:      return "not";
      case kVpiBufif0Prim:   return "bufif0";
      case kVpiBufif1Prim:   return "bufif1";
      case kVpiNotif0Prim:   return "notif0";
      case kVpiNotif1Prim:   return "notif1";
      case kVpiNmosPrim:     return "nmos";
      case kVpiPmosPrim:     return "pmos";
      case kVpiCmosPrim:     return "cmos";
      case kVpiRnmosPrim:    return "rnmos";
      case kVpiRpmosPrim:    return "rpmos";
      case kVpiRcmosPrim:    return "rcmos";
      case kVpiRtranPrim:    return "rtran";
      case kVpiRtranif0Prim: return "rtranif0";
      case kVpiRtranif1Prim: return "rtranif1";
      case kVpiTranPrim:     return "tran";
      case kVpiTranif0Prim:  return "tranif0";
      case kVpiTranif1Prim:  return "tranif1";
      case kVpiPullupPrim:   return "pullup";
      case kVpiPulldownPrim: return "pulldown";
      case kVpiSeqPrim:      return "seq";
      case kVpiCombPrim:     return "comb";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmOpTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] op_type 演算子のタイプ
VmOpTypeNode::VmOpTypeNode(tVpiOpType op_type) :
  mOpType(op_type)
{
}

// @brief デストラクタ
VmOpTypeNode::~VmOpTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmOpTypeNode::data(int column,
		   int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiOpType";
    }
    else if ( column == 1 ) {
      switch ( mOpType ) {
      case kVpiMinusOp:       return "minus";
      case kVpiPlusOp:        return "plus";
      case kVpiNotOp:         return "not";
      case kVpiBitNegOp:      return "bitneg";
      case kVpiUnaryAndOp:    return "unary and";
      case kVpiUnaryNandOp:   return "unary nand";
      case kVpiUnaryOrOp:     return "unary or";
      case kVpiUnaryNorOp:    return "unary nor";
      case kVpiUnaryXorOp:    return "unary xor";
      case kVpiUnaryXNorOp:   return "unary xnor";
      case kVpiSubOp:         return "sub";
      case kVpiDivOp:         return "div";
      case kVpiModOp:         return "mod";
      case kVpiEqOp:          return "eq";
      case kVpiNeqOp:         return "neq";
      case kVpiCaseEqOp:      return "caseeq";
      case kVpiCaseNeqOp:     return "caseneq";
      case kVpiGtOp:          return "gt";
      case kVpiGeOp:          return "ge";
      case kVpiLtOp:          return "lt";
      case kVpiLeOp:          return "le";
      case kVpiLShiftOp:      return "left shift";
      case kVpiRShiftOp:      return "right shift";
      case kVpiAddOp:         return "add";
      case kVpiMultOp:        return "mult";
      case kVpiLogAndOp:      return "logical and";
      case kVpiLogOrOp:       return "logical or";
      case kVpiBitAndOp:      return "bit and";
      case kVpiBitOrOp:       return "bit or";
      case kVpiBitXorOp:      return "bit xor";
      case kVpiBitXNorOp:     return "bit xnor";
      case kVpiConditionOp:   return "conditional";
      case kVpiConcatOp:      return "concat";
      case kVpiMultiConcatOp: return "multi concat";
      case kVpiEventOrOp:     return "event or";
      case kVpiNullOp:        return "null";
      case kVpiListOp:        return "list";
      case kVpiMinTypMaxOp:   return "min-typ-max";
      case kVpiPosedgeOp:     return "posedge";
      case kVpiNegedgeOp:     return "negedge";
      case kVpiArithLShiftOp: return "arithmetic left shift";
      case kVpiArithRShiftOp: return "arithmetic right shift";
      case kVpiPowerOp:       return "power";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmConstTypeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] const_type 定数の型
VmConstTypeNode::VmConstTypeNode(tVpiConstType const_type) :
  mConstType(const_type)
{
}

// @brief デストラクタ
VmConstTypeNode::~VmConstTypeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmConstTypeNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiConstType";
    }
    else if ( column == 1 ) {
      switch ( mConstType ) {
      case kVpiDecConst:          return "dec";
      case kVpiRealConst:         return "real";
      case kVpiBinaryConst:       return "binary";
      case kVpiOctConst:          return "oct";
      case kVpiHexConst:          return "hex";
      case kVpiStringConst:       return "string";
      case kVpiIntConst:          return "int";
      case kVpiSignedDecConst:    return "signed dec";
      case kVpiSignedBinaryConst: return "signed binary";
      case kVpiSignedOctConst:    return "signed oct";
      case kVpiSignedHexConst:    return "signed hex";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmRangeModeNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] range_mode 範囲指定の型
VmRangeModeNode::VmRangeModeNode(tVpiRangeMode range_mode) :
  mRangeMode(range_mode)
{
}

// @brief デストラクタ
VmRangeModeNode::~VmRangeModeNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmRangeModeNode::data(int column,
		      int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiRangeMode";
    }
    else if ( column == 1 ) {
      switch ( mRangeMode ) {
      case kVpiNoRange:    return "No Range";
      case kVpiConstRange: return "Constant Range";
      case kVpiPlusRange:  return "Plus Range";
      case kVpiMinusRange: return "Minus Range";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmStrengthValNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] strength 値を表すノード
VmStrengthValNode::VmStrengthValNode(const QString& label,
				     tVpiStrength strength) :
  mLabel(label),
  mStrength(strength)
{
}

// @brief デストラクタ
VmStrengthValNode::~VmStrengthValNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmStrengthValNode::data(int column,
			int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vpiStrength";
    }
    else if ( column == 1 ) {
      switch ( mStrength ) {
      case kVpiNoStrength:   return "none";
      case kVpiSupplyDrive:  return "supply";
      case kVpiStrongDrive:  return "strong";
      case kVpiPullDrive:    return "pull";
      case kVpiWeakDrive:    return "weak";
      case kVpiLargeCharge:  return "large";
      case kVpiMediumCharge: return "medium";
      case kVpiSmallCharge:  return "small";
      case kVpiHiZ:          return "high-Z";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmVsNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] vs vectored/scalared 情報
VmVsNode::VmVsNode(tVpiVsType vs) :
  mVs(vs)
{
}

// @brief デストラクタ
VmVsNode::~VmVsNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmVsNode::data(int column,
	       int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return "vectored/scalared";
    }
    else if ( column == 1 ) {
      switch ( mVs ) {
      case kVpiVsNone:   return "None";
      case kVpiVectored: return "Vectored";
      case kVpiScalared: return "Scalared";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmBoolNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] val 真理値
VmBoolNode::VmBoolNode(const QString& label,
		       bool val) :
  mLabel(label),
  mVal(val)
{
}

// @brief デストラクタ
VmBoolNode::~VmBoolNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmBoolNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      if ( mVal) {
	return "True";
      }
      else {
	return "False";
      }
    }
  }
  return QVariant();
}


//////////////////////////////////////////////////////////////////////
// クラス VmIntNode
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] val 値
VmIntNode::VmIntNode(const QString& label,
		     int val) :
  mLabel(label),
  mVal(val)
{
}

// @brief デストラクタ
VmIntNode::~VmBoolNode()
{
}

// @brief データを返す．
// @param[in] column コラム番号
// @param[in] role 
QVariant
VmIntNode::data(int column,
		 int role) const
{
  if ( role == Qt::DisplayRole ) {
    if ( column == 0 ) {
      return mLabel;
    }
    else if ( column == 1 ) {
      return mVal;
    }
  }
  return QVariant();
}

END_NAMESPACE_YM_VERILOG
