
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
  tVpiOpType op_type = pt_expr->op_type();
  ymuint opr_size = pt_expr->operand_num();

  ElbExpr* opr0 = NULL;
  ElbExpr* opr1 = NULL;
  ElbExpr* opr2 = NULL;
  ElbExpr* expr = NULL;
  bool real_check = false;
  switch ( op_type ) {
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    error_illegal_edge_descriptor(pt_expr);
    return NULL;

  case kVpiBitNegOp:
  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryXorOp:
  case kVpiUnaryXNorOp: real_check = true;
  case kVpiPlusOp:
  case kVpiMinusOp:
  case kVpiNotOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    if ( !opr0 ) {
      return NULL;
    }
    if ( real_check && opr0->value_type() == kVpiValueReal ) {
      error_illegal_real_type(pt_expr->operand(0));
      return NULL;
    }
    return factory().new_UnaryOp(pt_expr, op_type, opr0);

  case kVpiBitAndOp:
  case kVpiBitOrOp:
  case kVpiBitXNorOp:
  case kVpiBitXorOp:
  case kVpiLShiftOp:
  case kVpiRShiftOp:
  case kVpiArithLShiftOp:
  case kVpiArithRShiftOp: real_check = true;
  case kVpiAddOp:
  case kVpiSubOp:
  case kVpiMultOp:
  case kVpiDivOp:
  case kVpiModOp:
  case kVpiPowerOp:
  case kVpiLogAndOp:
  case kVpiLogOrOp:
  case kVpiCaseEqOp:
  case kVpiCaseNeqOp:
  case kVpiEqOp:
  case kVpiNeqOp:
  case kVpiGeOp:
  case kVpiGtOp:
  case kVpiLeOp:
  case kVpiLtOp:
    assert_cond(opr_size == 2, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    if ( !opr0 || !opr1 ) {
      return NULL;
    }
    if ( real_check ) {
      if ( opr0->value_type() == kVpiValueReal ) {
	error_illegal_real_type(pt_expr->operand(0));
	return NULL;
      }
      if ( opr1->value_type() == kVpiValueReal ) {
	error_illegal_real_type(pt_expr->operand(1));
	return NULL;
      }
    }
    expr = factory().new_BinaryOp(pt_expr, op_type, opr0, opr1);
    break;

  case kVpiConditionOp:
  case kVpiMinTypMaxOp:
    assert_cond(opr_size == 3, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    opr2 = instantiate_expr(parent, env, pt_expr->operand(2));
    if ( !opr0 || !opr1 || !opr2 ) {
      return NULL;
    }
    expr = factory().new_TernaryOp(pt_expr, op_type, opr0, opr1, opr2);
    break;

  case kVpiConcatOp:
    {
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for (ymuint i = 0; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type() == kVpiValueReal ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i] = expr1;
      }

      expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();
    }
    break;

  case kVpiMultiConcatOp:
    {
      const PtExpr* pt_expr0 = pt_expr->operand(0);
      int rep_num;
      bool stat = evaluate_int(parent, pt_expr0, rep_num, true);
      if ( !stat ) {
	return NULL;
      }
      ElbExpr** opr_list = factory().new_ExprList(opr_size - 1);
      for (ymuint i = 1; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_expr(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	if ( expr1->value_type() == kVpiValueReal ) {
	  error_illegal_real_type(pt_expr1);
	  return NULL;
	}
	opr_list[i - 1] = expr1;
      }
      expr = factory().new_MultiConcatOp(pt_expr, pt_expr0, rep_num,
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
  tVpiOpType op_type = pt_expr->op_type();
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
  switch ( op_type ) {
  case kVpiPosedgeOp:
  case kVpiNegedgeOp:
    // この演算は使えない．
    error_illegal_edge_descriptor(pt_expr);
    return VlValue();

  case kVpiBitNegOp:
  case kVpiUnaryAndOp:
  case kVpiUnaryNandOp:
  case kVpiUnaryOrOp:
  case kVpiUnaryNorOp:
  case kVpiUnaryXorOp:
  case kVpiUnaryXNorOp:
  case kVpiBitAndOp:
  case kVpiBitOrOp:
  case kVpiBitXNorOp:
  case kVpiBitXorOp:
  case kVpiLShiftOp:
  case kVpiRShiftOp:
  case kVpiArithLShiftOp:
  case kVpiArithRShiftOp:
  case kVpiModOp:
  case kVpiConcatOp:
  case kVpiMultiConcatOp:
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

  case kVpiPlusOp:
  case kVpiMinusOp:
  case kVpiAddOp:
  case kVpiSubOp:
  case kVpiMultOp:
  case kVpiDivOp:
  case kVpiPowerOp:
  case kVpiCaseEqOp:
  case kVpiCaseNeqOp:
  case kVpiEqOp:
  case kVpiNeqOp:
  case kVpiGeOp:
  case kVpiGtOp:
  case kVpiLeOp:
  case kVpiLtOp:
  case kVpiLogAndOp:
  case kVpiLogOrOp:
  case kVpiConditionOp:
  case kVpiMinTypMaxOp:
    // この演算はどの型でもOK
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    return VlValue();
  }

  // 二回目は値の評価を行う．
  switch ( op_type ) {
  case kVpiUnaryAndOp:
    return reduction_and(val[0]);

  case kVpiUnaryNandOp:
    return reduction_nand(val[0]);

  case kVpiUnaryOrOp:
    return reduction_or(val[0]);

  case kVpiUnaryNorOp:
    return reduction_nor(val[0]);

  case kVpiUnaryXorOp:
    return reduction_xor(val[0]);

  case kVpiUnaryXNorOp:
    return reduction_xor(val[0]);

  case kVpiPlusOp:
    return val[0];

  case kVpiMinusOp:
    return - val[0];

  case kVpiBitNegOp:
    return bit_negate(val[0]);

  case kVpiBitAndOp:
    return bit_and(val[0], val[1]);

  case kVpiBitOrOp:
    return bit_or(val[0], val[1]);

  case kVpiBitXNorOp:
    return bit_xnor(val[0], val[1]);

  case kVpiBitXorOp:
    return bit_xor(val[0], val[1]);

  case kVpiLShiftOp:
    return val[0] << val[1];

  case kVpiRShiftOp:
    return val[0] >> val[1];

  case kVpiArithLShiftOp:
    return alshift(val[0], val[1]);

  case kVpiArithRShiftOp:
    return arshift(val[0], val[1]);

  case kVpiAddOp:
    return val[0] + val[1];

  case kVpiSubOp:
    return val[0] - val[1];

  case kVpiMultOp:
    return val[0] * val[1];

  case kVpiDivOp:
    return val[0] / val[1];

  case kVpiModOp:
    return val[0] % val[1];

  case kVpiPowerOp:
    return power(val[0], val[1]);

  case kVpiNotOp:
    return log_not(val[0]);

  case kVpiLogAndOp:
    return log_and(val[0], val[1]);

  case kVpiLogOrOp:
    return log_or(val[0], val[1]);

  case kVpiCaseEqOp:
    return eq_with_x(val[0], val[1]);

  case kVpiCaseNeqOp:
    return log_not(eq_with_x(val[0], val[1]));

  case kVpiEqOp:
    return eq(val[0], val[1]);

  case kVpiNeqOp:
    return log_not(eq(val[0], val[1]));

  case kVpiGeOp:
    return ge(val[0], val[1]);

  case kVpiGtOp:
    return log_not(ge(val[1], val[0]));

  case kVpiLeOp:
    return ge(val[1], val[0]);

  case kVpiLtOp:
    return log_not(ge(val[0], val[1]));

  case kVpiConditionOp:
    return ite(val[0], val[1], val[2]);

  case kVpiMinTypMaxOp:
    // 本当はエラー
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kVpiConcatOp:
    return concat(val);

  case kVpiMultiConcatOp:
    return multi_concat(val);

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  // ダミー
  return VlValue();
}

END_NAMESPACE_YM_VERILOG
