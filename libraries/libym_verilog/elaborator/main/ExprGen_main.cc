
/// @file libym_verilog/elaborator/ExprGen_main.cc
/// @brief ExprGen の実装ファイル(メイン)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// $Id: ExprGen_main.cc 2507 2009-10-17 16:24:02Z matsunaga $
///
/// Copyright (C) 2005-2008 Yusuke Matsunaga
/// All rights reserved.


#if HAVE_CONFIG_H
#include <ymconfig.h>
#endif

#include "ExprGen.h"
#include "ElbEnv.h"

#include <ym_verilog/pt/PtItem.h>
#include <ym_verilog/pt/PtExpr.h>
#include <ym_verilog/pt/PtMisc.h>

#include "ElbExpr.h"


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
	  pt_expr->opr_type() == kVpiNullOp ) {
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
      // constant function 中で system function は使えない．
      ostringstream buf;
      buf << pt_expr->name()
	  << " : system-function call shall not be used in constant function.";
      put_msg(__FILE__, __LINE__,
	      pt_expr->file_region(),
	      kMsgError,
	      "ELAB",
	      buf.str());
      return NULL;
    }
    if ( env.is_constant() ) {
      // constant expression 中では system function は使えない．
      ostringstream buf;
      buf << pt_expr->name()
	  << " : system-function call shall not be used in constant expresson.";
      put_msg(__FILE__, __LINE__,
	      pt_expr->file_region(),
	      kMsgError,
	      "ELAB",
	      buf.str());
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
	  pt_expr->opr_type() == kVpiNullOp ) {
    pt_expr = pt_expr->operand(0);
  }
  
  const FileRegion& fr = pt_expr->file_region();

  switch ( pt_expr->type() ) {
  case kPtOprExpr:
    switch ( pt_expr->opr_type() ) {
    case kVpiPosedgeOp:
    case kVpiNegedgeOp:
      { // これのみがイベント式の特徴
	assert_cond(pt_expr->operand_num() == 1, __FILE__, __LINE__);
	ElbExpr* opr0 = instantiate_expr(parent, env, pt_expr->operand(0));
	if ( !opr0 ) {
	  return NULL;
	}
	ElbExpr* expr = factory().new_UnaryOp(pt_expr,
					      pt_expr->opr_type(), opr0);
  
	// attribute instance の生成
	//instantiate_attribute(pt_expr->attr_top(), false, expr);
	
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
      ElbEventEnv env1(env);
      return instantiate_primary(parent, env1, pt_expr);
    }
    
  case kPtConstExpr:
    // イベント式の根元には定数は使えない．
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "ELAB",
	    "Constant shall not be used in event description.");
    return NULL;

  case kPtFuncCallExpr:
    // イベント式の根元には関数呼び出しは使えない．
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "ELAB",
	    "Function call shall not be used in event description.");
    return NULL;

  case kPtSysFuncCallExpr:
    // イベント式の根元にはシステム関数呼び出しは使えない．
    {
      ostringstream buf;
      buf << pt_expr->name()
	  << " : system-function call shall not be used in event description.";
      put_msg(__FILE__, __LINE__,
	      fr,
	      kMsgError,
	      "ELAB",
	      buf.str());
    }
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
	  pt_expr->opr_type() == kVpiNullOp ) {
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
  const FileRegion& fr = pt_expr->file_region();

  switch ( pt_expr->type() ) {
  case kPtOprExpr:
    // 左辺では concatination しか適当でない．
    if ( pt_expr->opr_type() == kVpiConcatOp ) {
      ymuint32 opr_size = pt_expr->operand_num();
      ElbExpr** opr_list = factory().new_ExprList(opr_size);
      for (ymuint32 i = 0; i < opr_size; ++ i) {
	const PtExpr* pt_expr1 = pt_expr->operand(i);
	ElbExpr* expr1 = instantiate_lhs(parent, env, pt_expr1);
	if ( !expr1 ) {
	  return NULL;
	}
	opr_list[i] = expr1;
      }
      ElbExpr* expr = factory().new_ConcatOp(pt_expr, opr_size, opr_list);
      expr->set_selfsize();
  
      // attribute instance の生成
      //instantiate_attribute(pt_expr->attr_top(), false, expr);
	
      return expr;
    }
    put_msg(__FILE__, __LINE__,
	    pt_expr->file_region(),
	    kMsgError,
	    "ELAB",
	    "Illegal operator in LHS.");
    return NULL;


  case kPtPrimaryExpr:
    return instantiate_lhs_primary(parent, env, pt_expr);

  case kPtConstExpr:
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "ELAB",
	    "Constant shall not be used in LHS.");
    return NULL;
    
  case kPtFuncCallExpr:
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "ELAB",
	    "Function call shall not be used in LHS.");
    return NULL;
    
  case kPtSysFuncCallExpr:
    put_msg(__FILE__, __LINE__,
	    fr,
	    kMsgError,
	    "ELAB",
	    "System-function call shall not be used in LHS.");
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
// @note 定数でなければエラーメッセージを出力し false を返す．
// @brief PtExpr から expression を生成し int 値を返す．
bool
ExprGen::evaluate_expr_int(const VlNamedObj* parent,
			   const PtExpr* pt_expr,
			   int& value)
{
  ElbExpr* expr = instantiate_constant_expr(parent, pt_expr);
  if ( !expr ) {
    return false;
  }
  bool stat = expr->eval_int(value);
  if ( !stat ) {
    ostringstream buf;
    buf << expr->decompile() << ": Integer value required.";
    put_msg(__FILE__, __LINE__,
	    expr->file_region(),
	    kMsgError,
	    "ELAB",
	    buf.str());
  }
  return stat;
}

// @brief PtExpr を評価し bool 値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_expr_bool(const VlNamedObj* parent,
			    const PtExpr* pt_expr,
			    bool& value)
{
  ElbExpr* expr = instantiate_constant_expr(parent, pt_expr);
  if ( !expr ) {
    return false;
  }
  value = expr->eval_scalar() == kVpiScalar1;
  // たしか vpi_expr は自動的に開放されるはず
  return true;
}

// @brief PtExpr を評価しビットベクタ値を返す．
// @param[in] parent 親のスコープ
// @param[in] pt_expr 式を表すパース木
// @param[out] value 評価値を格納する変数
// @note 定数でなければエラーメッセージを出力し false を返す．
bool
ExprGen::evaluate_expr_bitvector(const VlNamedObj* parent,
				 const PtExpr* pt_expr,
				 BitVector& value)
{
  ElbExpr* expr = instantiate_constant_expr(parent, pt_expr);
  if ( !expr ) {
    return false;
  }
  expr->eval_bitvector(value);
  // たしか vpi_expr は自動的に開放されるはず
  return true;
}

// PtDelay から ElbDelay を生成する．
ElbDelay*
ExprGen::instantiate_delay(const VlNamedObj* parent,
			   const PtDelay* pt_delay)
{
  ymuint32 n = 0;
  for ( ; n < 3; ++ n) {
    if ( pt_delay->value(n) == NULL ) break;
  }
  assert_cond(n > 0, __FILE__, __LINE__);

  // TODO : 環境の条件をチェック
  ElbEnv env;
  ElbExpr** expr_list = factory().new_ExprList(n);
  for (ymuint32 i = 0; i < n; ++ i) {
    const PtExpr* pt_expr = pt_delay->value(i);
    ElbExpr* expr = instantiate_expr(parent, env, pt_expr);
    if ( !expr ) {
      return NULL;
    }
    expr_list[i] = expr;
  }
  
  ElbDelay* delay = factory().new_Delay(pt_delay, n, expr_list);

  return delay;
}

// PtOrderedCon から ElbDelay を生成する．
// これは PtMuInst の前にある # つきの式がパラメータ割り当てなのか
// 遅延なのかわからないので PtOrderedCon で表していることによる．
ElbDelay*
ExprGen::instantiate_delay(const VlNamedObj* parent,
			   const PtItem* pt_header)
{
  ymuint32 n = pt_header->paramassign_array().size();
  if ( n > 2 ) {
    const PtConnection* pt_con = pt_header->paramassign_array()[1];
    put_msg(__FILE__, __LINE__,
	    pt_con->file_region(),
	    kMsgError,
	    "ELAB",
	    "Too many values in this field.");
    return NULL;
  }

  // TODO : 環境の条件をチェック
  ElbEnv env;
  ElbExpr** expr_list = factory().new_ExprList(n);
  for (ymuint32 i = 0; i < n; ++ i ) {
    const PtConnection* pt_con = pt_header->paramassign_array()[i];
    const PtExpr* pt_expr = pt_con->expr();
    ElbExpr* expr = instantiate_expr(parent, env, pt_expr);
    if ( !expr ) {
      return NULL;
    }
    expr_list[i] = expr;
  }
  
  ElbDelay* delay = factory().new_Delay(pt_header, n, expr_list);

  
  return delay;
}

END_NAMESPACE_YM_VERILOG
