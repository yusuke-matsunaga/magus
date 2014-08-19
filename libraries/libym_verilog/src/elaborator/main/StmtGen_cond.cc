
/// @file StmtGen_cond.cc
/// @brief ElbMgr の実装ファイル(conditional statement の実体化)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2010, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "StmtGen.h"
#include "ElbEnv.h"
#include "ElbStmt.h"
#include "ElbExpr.h"

#include "YmVerilog/pt/PtStmt.h"
#include "YmVerilog/pt/PtExpr.h"

#include "YmUtils/MsgMgr.h"


BEGIN_NAMESPACE_YM_VERILOG

//////////////////////////////////////////////////////////////////////
// 条件ステートメント関係の instantiate 関数
//////////////////////////////////////////////////////////////////////

// @brief if 文の生成を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_if(const VlNamedObj* parent,
			ElbProcess* process,
			const ElbEnv& env,
			const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  if ( !cond ) {
    // たぶんエラー
    return NULL;
  }

  const PtStmt* pt_then = pt_stmt->body();
  ElbStmt* then_stmt = instantiate_stmt(parent, process, env, pt_then);
  if ( !then_stmt ) {
    // たぶんエラー
    return NULL;
  }

  ElbStmt* else_stmt = NULL;
  const PtStmt* pt_else = pt_stmt->else_body();
  if ( pt_else ) {
    else_stmt = instantiate_stmt(parent, process, env, pt_else);
    if ( !else_stmt ) {
      // たぶんエラー
      return NULL;
    }
  }

  ElbStmt* stmt = factory().new_IfStmt(parent, process, pt_stmt,
				       cond, then_stmt, else_stmt);

  return stmt;
}

// @brief case 文の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_case(const VlNamedObj* parent,
			  ElbProcess* process,
			  const ElbEnv& env,
			  const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());

  if ( !cond ) {
    // たぶんエラー
    return NULL;
  }

  ElbStmt* stmt = factory().new_CaseStmt(parent, process, pt_stmt, cond);

  // この case 文に関係する全ての式のリスト
  vector<ElbExpr*> expr_list;
  ymuint ne = 0;
  for (ymuint i = 0; i < pt_stmt->caseitem_num(); ++ i) {
    const PtCaseItem* pt_item = pt_stmt->caseitem(i);
    ne += pt_item->label_num();
  }
  expr_list.reserve(ne);

  // default caseitem を末尾にするために順序づけを行う．
  // Parser::check_default_label() で default が高々1個しかないことは確認済み．
  ymuint nc = pt_stmt->caseitem_num();
  vector<const PtCaseItem*> caseitem_list(nc);
  ymuint wpos = 0;
  for (ymuint i = 0; i < nc; ++ i) {
    const PtCaseItem* pt_item = pt_stmt->caseitem(i);
    if ( pt_item->label_num() > 0 ) {
      caseitem_list[wpos] = pt_item;
      ++ wpos;
    }
    else {
      caseitem_list[nc - 1] = pt_item;
    }
  }

  // case-item の生成
  for (ymuint i = 0; i < nc; ++ i) {
    const PtCaseItem* pt_item = caseitem_list[i];
    ElbStmt* body = NULL;
    if ( pt_item->body() ) {
      body = instantiate_stmt(parent, process, env, pt_item->body());
      if ( !body ) {
	return NULL;
      }
    }

    // ラベルの生成と設定
    ymuint n = pt_item->label_num();
    ElbExpr** label_list = factory().new_ExprList(n);
    for (ymuint j = 0; j < n; ++ j) {
      const PtExpr* pt_expr = pt_item->label(j);
      ElbExpr* tmp = instantiate_expr(parent, env, pt_expr);
      if ( !tmp ) {
	// たぶんエラー
	return NULL;
      }
      label_list[j] = tmp;
      expr_list.push_back(tmp);
    }

    stmt->set_caseitem(i, pt_item, label_list, body);
  }

  // expr_list のサイズを合わせる．
  // ルールは
  // - どれか一つでも符号付きならすべて符号付き
  // - サイズは各要素の最大サイズ
  VlValueType value_type0 = cond->value_type();
  if ( value_type0.is_real_type() ) {
    MsgMgr::put_msg(__FILE__, __LINE__,
		    cond->file_region(),
		    kMsgError,
		    "ELAB",
		    "Case expression should not be real-type.");
    return NULL;
  }
  bool sign = value_type0.is_signed();
  ymuint size = value_type0.size();
  for (vector<ElbExpr*>::iterator p = expr_list.begin();
       p != expr_list.end(); ++ p) {
    ElbExpr* expr = *p;
    VlValueType value_type1 = expr->value_type();
    if ( value_type1.is_real_type() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      expr->file_region(),
		      kMsgError,
		      "ELAB",
		      "Case-item expression should not be real-type.");
      return NULL;
    }
    if ( value_type1.is_signed() ) {
      sign = true;
    }
    ymuint size1 = value_type1.size();
    if ( size < size1 ) {
      size = size1;
    }
  }

  VlValueType value_type(sign, true, size);
  cond->set_reqsize(value_type);
  for (vector<ElbExpr*>::iterator p = expr_list.begin();
       p != expr_list.end(); ++ p) {
    ElbExpr* expr = *p;
    expr->set_reqsize(value_type);
  }

  return stmt;
}

// @brief wait 文の実体化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_wait(const VlNamedObj* parent,
			  ElbProcess* process,
			  const ElbEnv& env,
			  const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  ElbStmt* body = instantiate_stmt(parent, process, env,
				   pt_stmt->body());

  if ( !cond || !body ) {
    return NULL;
  }

  ElbStmt* stmt = factory().new_WaitStmt(parent, process, pt_stmt,
					 cond, body);

  return stmt;
}

// @brief forever 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_forever(const VlNamedObj* parent,
			     ElbProcess* process,
			     const ElbEnv& env,
			     const PtStmt* pt_stmt)
{
  ElbStmt* body = instantiate_stmt(parent, process, env, pt_stmt->body());

  if ( !body ) {
    return NULL;
  }

  ElbStmt* stmt = factory().new_ForeverStmt(parent, process, pt_stmt,
					    body);

  return stmt;
}

// @brief repeat 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_repeat(const VlNamedObj* parent,
			    ElbProcess* process,
			    const ElbEnv& env,
			    const PtStmt* pt_stmt)
{
  ElbExpr* expr = instantiate_expr(parent, env, pt_stmt->expr());

  ElbStmt* body = instantiate_stmt(parent, process, env, pt_stmt->body());

  if ( !expr || !body ) {
    return NULL;
  }

  ElbStmt* stmt = factory().new_RepeatStmt(parent, process, pt_stmt,
					   expr, body);

  return stmt;
}

// @brief while 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_while(const VlNamedObj* parent,
			   ElbProcess* process,
			   const ElbEnv& env,
			   const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());

  ElbStmt* body = instantiate_stmt(parent, process, env, pt_stmt->body());

  if ( !cond || !body ) {
    return NULL;
  }

  ElbStmt* stmt = factory().new_WhileStmt(parent, process, pt_stmt,
					  cond, body);

  return stmt;
}

// @brief for 文のインスタンス化を行う．
// @param[in] parent 親のスコープ
// @param[in] process 親のプロセス (or NULL)
// @param[in] env 生成時の環境
// @param[in] pt_stmt 対象のステートメント
ElbStmt*
StmtGen::instantiate_for(const VlNamedObj* parent,
			 ElbProcess* process,
			 const ElbEnv& env,
			 const PtStmt* pt_stmt)
{
  ElbExpr* cond = instantiate_expr(parent, env, pt_stmt->expr());
  ElbStmt* init = instantiate_stmt(parent, process, env,
				   pt_stmt->init_stmt());
  ElbStmt* next = instantiate_stmt(parent, process, env,
				   pt_stmt->next_stmt());
  ElbStmt* body = instantiate_stmt(parent, process, env,
				   pt_stmt->body());

  if ( !cond || !init || !next || !body ) {
    return NULL;
  }

  ElbStmt* stmt = factory().new_ForStmt(parent, process, pt_stmt,
					cond, init, next, body);

  return stmt;
}

END_NAMESPACE_YM_VERILOG
