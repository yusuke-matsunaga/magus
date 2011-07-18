#ifndef YM_VERILOG_VLOPTYPE_H
#define YM_VERILOG_VLOPTYPE_H

/// @file ym_verilog/VlOpType.h
/// @brief VlOpType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: verilog.h 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_verilog/verilog.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
/// @class VlOpType VlOpType.h "ym_verilog/VlOpType.h"
/// @brief operation の型
//////////////////////////////////////////////////////////////////////
class VlOpType
{
public:

  /// @brief vpi_user.h の定義値からの変換コンストラクタ
  explicit
  VlOpType(int op_val);


public:

  /// @brief vpi_user.h の定義値を返す．
  int
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // vpi_user.h の定義値
  ymuint8 mVal;

};


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief vpi_user.h の定義値からの変換コンストラクタ
inline
VlOpType::VlOpType(int op_val) :
  mVal(op_val)
{
}

// @brief vpi_user.h の定義値を返す．
inline
int
VlOpType::val() const
{
  return mVal;
}


#if 0
  kVpiMinusOp        = vpiMinusOp,
  kVpiPlusOp         = vpiPlusOp,
  kVpiNotOp          = vpiNotOp,
  kVpiBitNegOp       = vpiBitNegOp,
  kVpiUnaryAndOp     = vpiUnaryAndOp,
  kVpiUnaryNandOp    = vpiUnaryNandOp,
  kVpiUnaryOrOp      = vpiUnaryOrOp,
  kVpiUnaryNorOp     = vpiUnaryNorOp,
  kVpiUnaryXorOp     = vpiUnaryXorOp,
  kVpiUnaryXNorOp    = vpiUnaryXNorOp,
  kVpiSubOp          = vpiSubOp,
  kVpiDivOp          = vpiDivOp,
  kVpiModOp          = vpiModOp,
  kVpiEqOp           = vpiEqOp,
  kVpiNeqOp          = vpiNeqOp,
  kVpiCaseEqOp       = vpiCaseEqOp,
  kVpiCaseNeqOp      = vpiCaseNeqOp,
  kVpiGtOp           = vpiGtOp,
  kVpiGeOp           = vpiGeOp,
  kVpiLtOp           = vpiLtOp,
  kVpiLeOp           = vpiLeOp,
  kVpiLShiftOp       = vpiLShiftOp,
  kVpiRShiftOp       = vpiRShiftOp,
  kVpiAddOp          = vpiAddOp,
  kVpiMultOp         = vpiMultOp,
  kVpiLogAndOp       = vpiLogAndOp,
  kVpiLogOrOp        = vpiLogOrOp,
  kVpiBitAndOp       = vpiBitAndOp,
  kVpiBitOrOp        = vpiBitOrOp,
  kVpiBitXorOp       = vpiBitXorOp,
  kVpiBitXNorOp      = vpiBitXNorOp,
  kVpiConditionOp    = vpiConditionOp,
  kVpiConcatOp       = vpiConcatOp,
  kVpiMultiConcatOp  = vpiMultiConcatOp,
  kVpiEventOrOp      = vpiEventOrOp,
  kVpiNullOp         = vpiNullOp,
  kVpiListOp         = vpiListOp,
  kVpiMinTypMaxOp    = vpiMinTypMaxOp,
  kVpiPosedgeOp      = vpiPosedgeOp,
  kVpiNegedgeOp      = vpiNegedgeOp,
  kVpiArithLShiftOp  = vpiArithLShiftOp,
  kVpiArithRShiftOp  = vpiArithRShiftOp,
  kVpiPowerOp        = vpiPowerOp
#endif

END_NAMESPACE_YM_VERILOG

#endif // YM_VERILOG_VLOPTYPE_H
