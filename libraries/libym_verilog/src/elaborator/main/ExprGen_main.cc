
/// @file ExprGen_main.cc
/// @brief ExprGen の実装ファイル(メイン)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ExprGen.h"
#include "ElbEnv.h"

#include "YmVerilog/BitVector.h"
#include "YmVerilog/pt/PtItem.h"
#include "YmVerilog/pt/PtExpr.h"
#include "YmVerilog/pt/PtMisc.h"

#include "ElbExpr.h"

#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// クラス ExprGen
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] elab 生成器
// @param[in] elb_mgr Elbオブジェクトを管理するクラス
// @param[in] elb_factory Elbオブジェクトを生成するファクトリクラス
ExprGen::ExprGen(Elaborator& elab,
		 ElbMgr& elb_mgr,
		 ElbFactory& elb_factory) :
  ElbProxy(elab, elb_mgr, elb_factory)
{
}

// @brief デストラクタ
ExprGen::~ExprGen()
{
}

// @brief PtExpr から ElbiExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ExprGen::instantiate_expr(const VlNamedObj* parent,
			  const ElbEnv& env,
			  const PtExpr* pt_expr)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == kPtOprExpr &&
	  pt_expr->op_type() == kVlNullOp ) {
    pt_expr = pt_expr->operand(0);
  }

  switch ( pt_expr->type() ) {
  case kPtOprExpr:
    return instantiate_opr(parent, env, pt_expr);

  case kPtConstExpr:
    return factory().new_Constant(pt_expr);

  case kPtFuncCallExpr:
    return instantiate_funccall(parent, env, pt_expr);

  case kPtSysFuncCallExpr:
    if ( env.inside_constant_function() ) {
      error_illegal_sysfunccall_in_cf(pt_expr);
      return NULL;
    }
    if ( env.is_constant() ) {
      error_illegal_sysfunccall_in_ce(pt_expr);
      return NULL;
    }
    return instantiate_sysfunccall(parent, env, pt_expr);

  case kPtPrimaryExpr:
    return instantiate_primary(parent, env, pt_expr);

  default:
    assert_not_reached(__FILE__, __LINE__);
  }

  return NULL;
}

// @brief PtExpr から定数式の ElbExpr を生成する
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ExprGen::instantiate_constant_expr(const VlNamedObj* parent,
				   const PtExpr* pt_expr)
{
  ElbConstantEnv env;
  return instantiate_expr(parent, env, pt_expr);
}

// @brief PtExpr からイベント式を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ExprGen::instantiate_event_expr(const VlNamedObj* parent,
				const ElbEnv& env,
				const PtExpr* pt_expr)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == kPtOprExpr &&
	  pt_expr->op_type() == kVlNullOp ) {
    pt_expr = pt_expr->operand(0);
  }

  switch ( pt_expr->type() ) {
  case kPtOprExpr:
    switch ( pt_expr->op_type() ) {
    case kVlPosedgeOp:
    case kVlNegedgeOp:
      { // これのみがイベント式の特徴
	assert_cond(pt_expr->operand_num() == 1, __FILE__, __LINE__);
	ElbExpr* opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
	if ( !opr0 ) {
	  return NULL;
	}
	ElbExpr* expr = factory().new_UnaryOp(pt_expr,
					      pt_expr->op_type(), opr0);

#if 0
	// attribute instance の生成
	instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

	return expr;
      }

    default:
      // それ以外の場合は通常の処理を行う．
      return instantiate_opr(parent, env, pt_expr);
    }
    break;

  case kPtPrimaryExpr:
    // 通常の識別子に加えて named event も扱う．
    {
      ElbEventExprEnv env1(env);
      return instantiate_primary(parent, env1, pt_expr);
    }

  case kPtConstExpr:
    // イベント式の根元には定数は使えない．
    error_illegal_constant_in_event_expression(pt_expr);
    return NULL;

  case kPtFuncCallExpr:
    // イベント式の根元には関数呼び出しは使えない．
    error_illegal_funccall_in_event_expression(pt_expr);
    return NULL;

  case kPtSysFuncCallExpr:
    // イベント式の根元にはシステム関数呼び出しは使えない．
    error_illegal_sysfunccall_in_event_expression(pt_expr);
    return NULL;

  default:
    break;
  }

  assert_not_reached(__FILE__, __LINE__);

  return NULL;
}

// @brief PtExpr からシステム関数の引数を生成する．
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ExprGen::instantiate_arg(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == kPtOprExpr &&
	  pt_expr->op_type() == kVlNullOp ) {
    pt_expr = pt_expr->operand(0);
  }

  if ( pt_expr->type() == kPtPrimaryExpr ) {
    // システム関数の引数用の特別処理はここだけ．
    ElbSystemTfArgEnv env1(env);
    return instantiate_primary(parent, env1, pt_expr);
  }
  return instantiate_expr(parent, env, pt_expr);
}

// @brief PtExpr から左辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @return 生成された ElbExpr のポインタを返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ExprGen::instantiate_lhs(const VlNamedObj* parent,
			 const ElbEnv& env,
			 const PtExpr* pt_expr)
{
  switch ( pt_expr->type() ) {
  case kPtOprExpr:
    // 左辺では concatination しか適当でない．
    if ( pt_expr->op_type() == kVlConcatOp ) {
      vector<ElbExpr*> elem_array;
      ymuint opr_size = pt_expr->operand_num();
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for (ymuint i = 0; i < opr_size; ++ i) {
	ymuint pos = opr_size - i - 1;
	const PtExpr* pt_expr1 = pt_expr->operand(pos);
	vector<ElbExpr*> tmp_array;
	ElbExpr* expr1 = instantiate_lhs_sub(parent, env, pt_expr1, tmp_array);
	if ( !expr1 ) {
	  return NULL;
	}
	opr_list[pos] = expr1;
	elem_array.insert(elem_array.end(), tmp_array.begin(), tmp_array.end());
      }
      ymuint n = elem_array.size();
      ElbExpr** lhs_elem_array = factory().new_ExprList(n);
      for (ymuint i = 0; i < n; ++ i) {
	lhs_elem_array[i] = elem_array[i];
      }
      ElbExpr* expr = factory().new_Lhs(pt_expr, opr_size, opr_list,
					n, lhs_elem_array);
      expr->set_selfsize();

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

      return expr;
    }
    // それ以外の演算子はエラー
    error_illegal_operator_in_lhs(pt_expr);
    return NULL;


  case kPtPrimaryExpr:
    return instantiate_primary(parent, env, pt_expr);

  case kPtConstExpr:
    error_illegal_constant_in_lhs(pt_expr);
    return NULL;

  case kPtFuncCallExpr:
    error_illegal_funccall_in_lhs(pt_expr);
    return NULL;

  case kPtSysFuncCallExpr:
    error_illegal_sysfunccall_in_lhs(pt_expr);
    return NULL;

  default:
    break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief PtExpr から左辺式を生成する
// @param[in] parent 親のスコープ
// @param[in] env 生成時の環境
// @param[in] pt_expr 式を表すパース木
// @param[out] elem_array 生成した左辺式の要素を格納するベクタ
// @return 生成した式を返す．
// @note 不適切な式ならばエラーメッセージを出力し NULL を返す．
ElbExpr*
ExprGen::instantiate_lhs_sub(const VlNamedObj* parent,
			     const ElbEnv& env,
			     const PtExpr* pt_expr,
			     vector<ElbExpr*>& elem_array)
{
  elem_array.clear();
  switch ( pt_expr->type() ) {
  case kPtOprExpr:
    // 左辺では concatination しか適当でない．
    if ( pt_expr->op_type() == kVlConcatOp ) {
      ymuint opr_size = pt_expr->operand_num();
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for (ymuint i = 0; i < opr_size; ++ i) {
	ymuint pos = opr_size - i - 1;
	const PtExpr* pt_expr1 = pt_expr->operand(pos);
	vector<ElbExpr*> tmp_array;
	ElbExpr* expr1 = instantiate_lhs_sub(parent, env, pt_expr1, tmp_array);
	if ( !expr1 ) {
	  return NULL;
	}
	opr_list[pos] = expr1;
	elem_array.insert(elem_array.end(), tmp_array.begin(), tmp_array.end());
      }
      ElbExpr* expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();

#if 0
      // attribute instance の生成
      instantiate_attribute(pt_expr->attr_top(), false, expr);
#else
#warning "TODO:2011-02-09-01"
#endif

      return expr;
    }
    // それ以外の演算子はエラー
    error_illegal_operator_in_lhs(pt_expr);
    return NULL;


  case kPtPrimaryExpr:
    {
      ElbExpr* expr = instantiate_primary(parent, env, pt_expr);
      elem_array.push_back(expr);
      return expr;
    }

  case kPtConstExpr:
    error_illegal_constant_in_lhs(pt_expr);
    return NULL;

  case kPtFuncCallExpr:
    error_illegal_funccall_in_lhs(pt_expr);
    return NULL;

  case kPtSysFuncCallExpr:
    error_illegal_sysfunccall_in_lhs(pt_expr);
    return NULL;

  default:
    break;
  }

  assert_not_reached(__FILE__, __LINE__);
  return NULL;
}

// @brief PtExpr を評価し int 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
// @brief PtExpr から expression を生成し int 値を返す．
bool
ExprGen::evaluate_int(const VlNamedObj* parent,
		      const PtExpr* pt_expr,
		      int& value,
		      bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  if ( !val.is_int_conv() ) {
    if ( put_error ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_expr->file_region(),
		      kMsgError,
		      "ELAB",
		      "Integer value required.");
    }
    return false;
  }

  value = val.int_value();
  return true;
}

// @brief PtExpr を評価しスカラー値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_scalar(const VlNamedObj* parent,
			 const PtExpr* pt_expr,
			 VlScalarVal& value,
			 bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  value = val.scalar_value();
  return true;
}

// @brief PtExpr を評価し bool 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_bool(const VlNamedObj* parent,
		       const PtExpr* pt_expr,
		       bool& value,
		       bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  value = val.logic_value().to_bool();
  return true;
}

// @brief PtExpr を評価しビットベクタ値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @param[in] put_error エラーを出力する時，true にする．
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_bitvector(const VlNamedObj* parent,
			    const PtExpr* pt_expr,
			    BitVector& value,
			    bool put_error)
{
  VlValue val = evaluate_expr(parent, pt_expr, put_error);
  if ( !val.is_bitvector_conv() ) {
    if ( put_error ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      pt_expr->file_region(),
		      kMsgError,
		      "ELAB",
		      "Bit-vector value required.");
    }
    return false;
  }

  value = val.bitvector_value();
  return true;
}

// @brief 式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[in] put_error エラーを出力する時，true にする．
VlValue
ExprGen::evaluate_expr(const VlNamedObj* parent,
		       const PtExpr* pt_expr,
		       bool put_error)
{
  // '(' expression ')' の時の対応
  while ( pt_expr->type() == kPtOprExpr &&
	  pt_expr->op_type() == kVlNullOp ) {
    pt_expr = pt_expr->operand(0);
  }

  switch ( pt_expr->type() ) {
  case kPtOprExpr:
    return evaluate_opr(parent, pt_expr, put_error);

  case kPtConstExpr:
    return evaluate_const(parent, pt_expr);

  case kPtFuncCallExpr:
    return evaluate_funccall(parent, pt_expr, put_error);

  case kPtSysFuncCallExpr:
    if ( put_error ) {
      error_illegal_sysfunccall_in_ce(pt_expr);
    }
    break;

  case kPtPrimaryExpr:
    return evaluate_primary(parent, pt_expr, put_error);

  default:
    assert_not_reached(__FILE__, __LINE__);
  }

  return VlValue();
}

// @brief 定数に対して式の値を評価する．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
VlValue
ExprGen::evaluate_const(const VlNamedObj* parent,
			const PtExpr* pt_expr)
{
  ymuint size = pt_expr->const_size();
  bool is_signed = false;
  ymuint base = 0;
  switch ( pt_expr->const_type() ) {
  case kVpiIntConst:
    if ( pt_expr->const_str() == NULL ) {
      return VlValue(static_cast<int>(pt_expr->const_uint()));
    }
    break;

  case kVpiRealConst:
    return VlValue(pt_expr->const_real());

  case kVpiSignedBinaryConst:
    is_signed = true;
  case kVpiBinaryConst:
    base = 2;
    break;

  case kVpiSignedOctConst:
    is_signed = true;
  case kVpiOctConst:
    base = 8;
    break;

  case kVpiSignedDecConst:
    is_signed = true;
  case kVpiDecConst:
    base = 10;
    break;

  case kVpiSignedHexConst:
    is_signed = true;
  case kVpiHexConst:
    base = 16;
    break;

  case kVpiStringConst:
    return VlValue(BitVector(pt_expr->const_str()));

  default:
    assert_not_reached(__FILE__, __LINE__);
    break;
  }

  // ここに来たということはビットベクタ型
  return VlValue(BitVector(size, is_signed, base, pt_expr->const_str()));
}

// @brief PtDelay から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_delay 遅延を表すパース木
ElbDelay*
ExprGen::instantiate_delay(const VlNamedObj* parent,
			   const PtDelay* pt_delay)
{
  ymuint n = 0;
  const PtExpr* expr_array[3];
  for ( ; n < 3; ++ n) {
    const PtExpr* expr = pt_delay->value(n);
    if ( expr == NULL ) break;
    expr_array[n] = expr;
  }
  assert_cond(n > 0, __FILE__, __LINE__);

  return instantiate_delay_sub(parent, pt_delay, n, expr_array);
}

// @brief PtOrderedCon から ElbExpr を生成する．
// @param[in] parent 親のスコープ
// @param[in] pt_head 順序付き割り当て式
// これは PtInst の前にある # つきの式がパラメータ割り当てなのか
// 遅延なのかわからないので PtOrderedCon で表していることによる．
ElbDelay*
ExprGen::instantiate_delay(const VlNamedObj* parent,
			   const PtItem* pt_header)
{
  ymuint n = pt_header->paramassign_array().size();
  assert_cond( n == 1, __FILE__, __LINE__);

  const PtExpr* expr_array[1];
  const PtConnection* pt_con = pt_header->paramassign_array()[0];
  expr_array[0] = pt_con->expr();
  return instantiate_delay_sub(parent, pt_header, 1, expr_array);
}

// @brief instantiate_delay の下請け関数
// @param[in] parent 親のスコープ
// @param[in] pt_obj 遅延式を表すパース木
// @param[in] n 要素数
// @param[in] expr_array 遅延式の配列
// @note pt_obj は PtDelay か PtItem のどちらか
// @note n は最大で 3
ElbDelay*
ExprGen::instantiate_delay_sub(const VlNamedObj* parent,
			       const PtBase* pt_obj,
			       ymuint n,
			       const PtExpr* expr_array[])
{
  assert_cond( n <= 3, __FILE__, __LINE__);

  // TODO : 環境の条件をチェック
  ElbEnv env;
  ElbExpr** expr_list = factory().new_ExprList(n);
  for (ymuint i = 0; i < n; ++ i) {
    const PtExpr* pt_expr = expr_array[i];
    ElbExpr* expr = instantiate_expr(parent, env, pt_expr);
    if ( !expr ) {
      return NULL;
    }
    expr_list[i] = expr;
  }

  ElbDelay* delay = factory().new_Delay(pt_obj, n, expr_list);

  return delay;
}

END_NAMESPACE_YM_VERILOG
