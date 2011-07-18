
/// @file libym_verilog/elaborator/ExprGen_opration.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_operation.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2010 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "ym_verilog/VlOpType.h"
#include "ym_verilog/BitVector.h"
#include "ym_verilog/pt/PtExpr.h"
#include "ym_verilog/pt/PtMisc.h"

#include "ElbExpr.h"


BEGIN_NAMESPACE_YM_VERILOG

// @brief PtOpr から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] env 生成時の環境
ElbExpr*
ExprGen::instantiate_opr(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  VlOpType op_type = pt_expr->op_type();
  ymuint opr_size = pt_expr->operand_num();

  ElbExpr* opr0 = NULL;
  ElbExpr* opr1 = NULL;
  ElbExpr* opr2 = NULL;
  ElbExpr* expr = NULL;
  bool real_check = false;
  switch ( op_type.val() ) {
  case vpiPosedgeOp:
  case vpiNegedgeOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    error_illegal_edge_descriptor(pt_expr);
    return NULL;

  case vpiBitNegOp:
  case vpiUnaryAndOp:
  case vpiUnaryNandOp:
  case vpiUnaryOrOp:
  case vpiUnaryNorOp:
  case vpiUnaryXorOp:
  case vpiUnaryXNorOp: real_check = true;
  case vpiPlusOp:
  case vpiMinusOp:
  case vpiNotOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    if ( !opr0 ) {
      return NULL;
    }
    if ( real_check && opr0->value_type().is_real_type() ) {
      error_illegal_real_type(pt_expr->operand(0));
      return NULL;
    }
    return factory().new_UnaryOp(pt_expr, op_type, opr0);

  case vpiBitAndOp:
  case vpiBitOrOp:
  case vpiBitXNorOp:
  case vpiBitXorOp:
  case vpiLShiftOp:
  case vpiRShiftOp:
  case vpiArithLShiftOp:
  case vpiArithRShiftOp: real_check = true;
  case vpiAddOp:
  case vpiSubOp:
  case vpiMultOp:
  case vpiDivOp:
  case vpiModOp:
  case vpiPowerOp:
  case vpiLogAndOp:
  case vpiLogOrOp:
  case vpiCaseEqOp:
  case vpiCaseNeqOp:
  case vpiEqOp:
  case vpiNeqOp:
  case vpiGeOp:
  case vpiGtOp:
  case vpiLeOp:
  case vpiLtOp:
    assert_cond(opr_size == 2, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    if ( !opr0 || !opr1 ) {
      return NULL;
    }
    if ( real_check ) {
      if ( opr0->value_type().is_real_type() ) {
	error_illegal_real_type(pt_expr->operand(0));
	return NULL;
      }
      if ( opr1->value_type().is_real_type() ) {
	error_illegal_real_type(pt_expr->operand(1));
	return NULL;
      }
    }
    expr = factory().new_BinaryOp(pt_expr, op_type, opr0, opr1);
    break;

  case vpiConditionOp:
  case vpiMinTypMaxOp:
    assert_cond(opr_size == 3, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    opr2 = instantiate_expr(parent, env, pt_expr->operand(2));
    if ( !opr0 || !opr1 || !opr2 ) {
      return NULL;
    }
    expr = factory().new_TernaryOp(pt_expr, op_type, opr0, opr1, opr2);
    break;

  case vpiConcatOp:
    {
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for (ymuint i = 0; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type().is_real_type() ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i] = expr1;
      }

      expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();
    }
    break;

  case vpiMultiConcatOp:
    {
      const PtExpr* pt_expr0 = pt_expr->operand(0);
      int rep_num;
      bool stat = evaluate_int(parent, pt_expr0, rep_num, true);
      if ( !stat ) {
	return NULL;
      }
      ElbExpr* rep_expr = instantiate_expr(parent, env, pt_expr0);
      ElbExpr** opr_list = factory().new_ExprList(opr_size - 1);
      for (ymuint i = 1; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type().is_real_type() ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i - 1] = expr1;
      }
      expr = factory().new_MultiConcatOp(pt_expr, rep_num, rep_expr,
					 opr_size - 1, opr_list);
      expr->set_selfsize();
    }
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    return NULL;
  }

#if 0
  // attribute instance の生成
  instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

  return expr;
}

// @brief 演算子に対して int 型の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] put_error エラーを出力する時，true にする．
VlValue
ExprGen::evaluate_opr(const VlNamedObj* parent,
		      const PtExpr* pt_expr,
		      bool put_error)
{
  VlOpType op_type = pt_expr->op_type();
  ymuint op_size = pt_expr->operand_num();
  vector<VlValue> val(3);

  // オペランドの値の評価を行う．
  for (ymuint i = 0; i < op_size; ++ i) {
    val[i] = evaluate_expr(parent, pt_expr->operand(i), put_error);
    if ( val[i].is_error() ) {
      return VlValue();
    }
  }

  // 結果の型のチェックを行う．
  switch ( op_type.val() ) {
  case vpiPosedgeOp:
  case vpiNegedgeOp:
    // この演算は使えない．
    error_illegal_edge_descriptor(pt_expr);
    return VlValue();

  case vpiBitNegOp:
  case vpiUnaryAndOp:
  case vpiUnaryNandOp:
  case vpiUnaryOrOp:
  case vpiUnaryNorOp:
  case vpiUnaryXorOp:
  case vpiUnaryXNorOp:
  case vpiBitAndOp:
  case vpiBitOrOp:
  case vpiBitXNorOp:
  case vpiBitXorOp:
  case vpiLShiftOp:
  case vpiRShiftOp:
  case vpiArithLShiftOp:
  case vpiArithRShiftOp:
  case vpiModOp:
  case vpiConcatOp:
  case vpiMultiConcatOp:
    // この演算はビットベクタ型に変換できなければならない．
    for (ymuint i = 0; i < op_size; ++ i) {
      if ( !val[i].is_bitvector_conv() ) {
	if ( put_error ) {
	  error_illegal_real_type(pt_expr->operand(i));
	}
	return VlValue();
      }
    }
    break;

  case vpiPlusOp:
  case vpiMinusOp:
  case vpiAddOp:
  case vpiSubOp:
  case vpiMultOp:
  case vpiDivOp:
  case vpiPowerOp:
  case vpiCaseEqOp:
  case vpiCaseNeqOp:
  case vpiEqOp:
  case vpiNeqOp:
  case vpiGeOp:
  case vpiGtOp:
  case vpiLeOp:
  case vpiLtOp:
  case vpiLogAndOp:
  case vpiLogOrOp:
  case vpiConditionOp:
  case vpiMinTypMaxOp:
    // この演算はどの型でもOK
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    return VlValue();
  }

  // 二回目は値の評価を行う．
  switch ( op_type.val() ) {
  case vpiUnaryAndOp:
    return reduction_and(val[0]);

  case vpiUnaryNandOp:
    return reduction_nand(val[0]);

  case vpiUnaryOrOp:
    return reduction_or(val[0]);

  case vpiUnaryNorOp:
    return reduction_nor(val[0]);

  case vpiUnaryXorOp:
    return reduction_xor(val[0]);

  case vpiUnaryXNorOp:
    return reduction_xor(val[0]);

  case vpiPlusOp:
    return val[0];

  case vpiMinusOp:
    return - val[0];

  case vpiBitNegOp:
    return bit_negate(val[0]);

  case vpiBitAndOp:
    return bit_and(val[0], val[1]);

  case vpiBitOrOp:
    return bit_or(val[0], val[1]);

  case vpiBitXNorOp:
    return bit_xnor(val[0], val[1]);

  case vpiBitXorOp:
    return bit_xor(val[0], val[1]);

  case vpiLShiftOp:
    return val[0] << val[1];

  case vpiRShiftOp:
    return val[0] >> val[1];

  case vpiArithLShiftOp:
    return alshift(val[0], val[1]);

  case vpiArithRShiftOp:
    return arshift(val[0], val[1]);

  case vpiAddOp:
    return val[0] + val[1];

  case vpiSubOp:
    return val[0] - val[1];

  case vpiMultOp:
    return val[0] * val[1];

  case vpiDivOp:
    return val[0] / val[1];

  case vpiModOp:
    return val[0] % val[1];

  case vpiPowerOp:
    return power(val[0], val[1]);

  case vpiNotOp:
    return log_not(val[0]);

  case vpiLogAndOp:
    return log_and(val[0], val[1]);

  case vpiLogOrOp:
    return log_or(val[0], val[1]);

  case vpiCaseEqOp:
    return eq_with_x(val[0], val[1]);

  case vpiCaseNeqOp:
    return log_not(eq_with_x(val[0], val[1]));

  case vpiEqOp:
    return eq(val[0], val[1]);

  case vpiNeqOp:
    return log_not(eq(val[0], val[1]));

  case vpiGeOp:
    return ge(val[0], val[1]);

  case vpiGtOp:
    return log_not(ge(val[1], val[0]));

  case vpiLeOp:
    return ge(val[1], val[0]);

  case vpiLtOp:
    return log_not(ge(val[0], val[1]));

  case vpiConditionOp:
    return ite(val[0], val[1], val[2]);

  case vpiMinTypMaxOp:
    // 本当はエラー
    assert_not_reached(__FILE__, __LINE__);
    break;

  case vpiConcatOp:
    return concat(val);

  case vpiMultiConcatOp:
    return multi_concat(val);

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  // ダミー
  return VlValue();
}

END_NAMESPACE_YM_VERILOG
