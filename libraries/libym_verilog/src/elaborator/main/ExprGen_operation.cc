
/// @file ExprGen_opration.cc
/// @brief ElbMgr の実装ファイル(式の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "YmVerilog/BitVector.h"
#include "YmVerilog/pt/PtExpr.h"
#include "YmVerilog/pt/PtMisc.h"

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
  tVlOpType op_type = pt_expr->op_type();
  ymuint opr_size = pt_expr->operand_num();

  ElbExpr* opr0 = NULL;
  ElbExpr* opr1 = NULL;
  ElbExpr* opr2 = NULL;
  ElbExpr* expr = NULL;
  bool real_check = false;
  switch ( op_type ) {
  case kVlPosedgeOp:
  case kVlNegedgeOp:
    assert_cond(opr_size == 1, __FILE__, __LINE__);
    error_illegal_edge_descriptor(pt_expr);
    return NULL;

  case kVlBitNegOp:
  case kVlUnaryAndOp:
  case kVlUnaryNandOp:
  case kVlUnaryOrOp:
  case kVlUnaryNorOp:
  case kVlUnaryXorOp:
  case kVlUnaryXNorOp: real_check = true;
  case kVlPlusOp:
  case kVlMinusOp:
  case kVlNotOp:
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

  case kVlBitAndOp:
  case kVlBitOrOp:
  case kVlBitXNorOp:
  case kVlBitXorOp:
  case kVlLShiftOp:
  case kVlRShiftOp:
  case kVlArithLShiftOp:
  case kVlArithRShiftOp: real_check = true;
  case kVlAddOp:
  case kVlSubOp:
  case kVlMultOp:
  case kVlDivOp:
  case kVlModOp:
  case kVlPowerOp:
  case kVlLogAndOp:
  case kVlLogOrOp:
  case kVlCaseEqOp:
  case kVlCaseNeqOp:
  case kVlEqOp:
  case kVlNeqOp:
  case kVlGeOp:
  case kVlGtOp:
  case kVlLeOp:
  case kVlLtOp:
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

  case kVlConditionOp:
  case kVlMinTypMaxOp:
    assert_cond(opr_size == 3, __FILE__, __LINE__);
    opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
    opr1 = instantiate_expr(parent, env, pt_expr->operand(1));
    opr2 = instantiate_expr(parent, env, pt_expr->operand(2));
    if ( !opr0 || !opr1 || !opr2 ) {
      return NULL;
    }
    expr = factory().new_TernaryOp(pt_expr, op_type, opr0, opr1, opr2);
    break;

  case kVlConcatOp:
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

  case kVlMultiConcatOp:
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
  tVlOpType op_type = pt_expr->op_type();
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
  case kVlPosedgeOp:
  case kVlNegedgeOp:
    // この演算は使えない．
    error_illegal_edge_descriptor(pt_expr);
    return VlValue();

  case kVlBitNegOp:
  case kVlUnaryAndOp:
  case kVlUnaryNandOp:
  case kVlUnaryOrOp:
  case kVlUnaryNorOp:
  case kVlUnaryXorOp:
  case kVlUnaryXNorOp:
  case kVlBitAndOp:
  case kVlBitOrOp:
  case kVlBitXNorOp:
  case kVlBitXorOp:
  case kVlLShiftOp:
  case kVlRShiftOp:
  case kVlArithLShiftOp:
  case kVlArithRShiftOp:
  case kVlModOp:
  case kVlConcatOp:
  case kVlMultiConcatOp:
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

  case kVlPlusOp:
  case kVlMinusOp:
  case kVlAddOp:
  case kVlSubOp:
  case kVlMultOp:
  case kVlDivOp:
  case kVlPowerOp:
  case kVlCaseEqOp:
  case kVlCaseNeqOp:
  case kVlEqOp:
  case kVlNeqOp:
  case kVlGeOp:
  case kVlGtOp:
  case kVlLeOp:
  case kVlLtOp:
  case kVlLogAndOp:
  case kVlLogOrOp:
  case kVlConditionOp:
  case kVlMinTypMaxOp:
    // この演算はどの型でもOK
    break;

  default:
    assert_not_reached(__FILE__, __LINE__);
    return VlValue();
  }

  // 二回目は値の評価を行う．
  switch ( op_type ) {
  case kVlUnaryAndOp:
    return reduction_and(val[0]);

  case kVlUnaryNandOp:
    return reduction_nand(val[0]);

  case kVlUnaryOrOp:
    return reduction_or(val[0]);

  case kVlUnaryNorOp:
    return reduction_nor(val[0]);

  case kVlUnaryXorOp:
    return reduction_xor(val[0]);

  case kVlUnaryXNorOp:
    return reduction_xor(val[0]);

  case kVlPlusOp:
    return val[0];

  case kVlMinusOp:
    return - val[0];

  case kVlBitNegOp:
    return bit_negate(val[0]);

  case kVlBitAndOp:
    return bit_and(val[0], val[1]);

  case kVlBitOrOp:
    return bit_or(val[0], val[1]);

  case kVlBitXNorOp:
    return bit_xnor(val[0], val[1]);

  case kVlBitXorOp:
    return bit_xor(val[0], val[1]);

  case kVlLShiftOp:
    return val[0] << val[1];

  case kVlRShiftOp:
    return val[0] >> val[1];

  case kVlArithLShiftOp:
    return alshift(val[0], val[1]);

  case kVlArithRShiftOp:
    return arshift(val[0], val[1]);

  case kVlAddOp:
    return val[0] + val[1];

  case kVlSubOp:
    return val[0] - val[1];

  case kVlMultOp:
    return val[0] * val[1];

  case kVlDivOp:
    return val[0] / val[1];

  case kVlModOp:
    return val[0] % val[1];

  case kVlPowerOp:
    return power(val[0], val[1]);

  case kVlNotOp:
    return log_not(val[0]);

  case kVlLogAndOp:
    return log_and(val[0], val[1]);

  case kVlLogOrOp:
    return log_or(val[0], val[1]);

  case kVlCaseEqOp:
    return eq_with_x(val[0], val[1]);

  case kVlCaseNeqOp:
    return log_not(eq_with_x(val[0], val[1]));

  case kVlEqOp:
    return eq(val[0], val[1]);

  case kVlNeqOp:
    return log_not(eq(val[0], val[1]));

  case kVlGeOp:
    return ge(val[0], val[1]);

  case kVlGtOp:
    return log_not(ge(val[1], val[0]));

  case kVlLeOp:
    return ge(val[1], val[0]);

  case kVlLtOp:
    return log_not(ge(val[0], val[1]));

  case kVlConditionOp:
    return ite(val[0], val[1], val[2]);

  case kVlMinTypMaxOp:
    // 本当はエラー
    assert_not_reached(__FILE__, __LINE__);
    break;

  case kVlConcatOp:
    return concat(val);

  case kVlMultiConcatOp:
    return multi_concat(val);

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  // ダミー
  return VlValue();
}

END_NAMESPACE_YM_VERILOG
