
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCompiler.h"
#include "AstEnumConst.h"
#include "AstExpr.h"
#include "AstMgr.h"
#include "AstParam.h"
#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstType.h"

#include "YmslScope.h"
#include "YmslFunction.h"
#include "YmslVar.h"

#include "YmslOpExpr.h"

#include "YmslMemberRef.h"
#include "YmslArrayRef.h"
#include "YmslFuncCall.h"
#include "YmslIntConst.h"
#include "YmslFloatConst.h"
#include "YmslStringConst.h"
#include "YmslTrue.h"
#include "YmslFalse.h"

#include "ObjHandle.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslCompiler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslCompiler::YmslCompiler()
{
}

// @brief デストラクタ
YmslCompiler::~YmslCompiler()
{
}

// @brief コンパイルする．
// @param[in] ido 入力データ
// @return 成功したら true を返す．
bool
YmslCompiler::compile(IDO& ido)
{
  AstMgr mgr;

  bool stat = mgr.read_source(ido);
  if ( !stat ) {
    return false;
  }

  AstStatement* toplevel = mgr.toplevel();
  YmslScope* toplevel_scope = new YmslScope(NULL, ShString("__main__"));

  mStmtList.clear();

  phase1(toplevel, toplevel_scope);

  for (vector<pair<const AstStatement*, YmslScope*> >::iterator p = mStmtList.begin();
       p != mStmtList.end(); ++ p) {
    const AstStatement* stmt = p->first;
    YmslScope* scope = p->second;
    phase2(stmt, scope);
  }

  return true;
}

// @brief 要素の生成を行う．
// @param[in] stmt 文
void
YmslCompiler::phase1(const AstStatement* stmt,
		     YmslScope* scope)
{
  mStmtList.push_back(make_pair(stmt, scope));

  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    {
      YmslScope* block_scope = new YmslScope(scope);

      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	phase1(stmt1, block_scope);
      }
    }
    break;

  case kBreak:
    break;

  case kContinue:
    break;

  case kDoWhile:
    phase1(stmt->stmt(), scope);
    break;

  case kEnumDecl:
    reg_enum(stmt, scope);
    break;

  case kEqAssign:
  case kEqPlus:
  case kEqMinus:
  case kEqMult:
  case kEqDiv:
  case kEqMod:
  case kEqLshift:
  case kEqRshift:
  case kEqAnd:
  case kEqOr:
  case kEqXor:
  case kExprStmt:
    break;

  case kFuncDecl:
    reg_func(stmt, scope);
    break;

  case kGoto:
    break;

  case kIf:
    phase1(stmt->stmt(), scope);
    phase1(stmt->else_stmt(), scope);
    break;

  case kImport:
    break;

  case kLabel:
    break;

  case kReturn:
    break;

  case kSwitch:
    break;

  case kToplevel:
    phase1(stmt->stmt(), scope);
    break;

  case kVarDecl:
    reg_var(stmt, scope);
    break;

  case kFor:
    {
      YmslScope* for_scope = new YmslScope(scope);
      phase1(stmt->init_stmt(), for_scope);
      phase1(stmt->next_stmt(), for_scope);
      phase1(stmt->stmt(), for_scope);
    }
    break;

  case kWhile:
    phase1(stmt->stmt(), scope);
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}

// @brief enum 型の定義を行う．
// @param[in] stmt 文
//
// stmt は kEnumDecl でなければならない．
void
YmslCompiler::reg_enum(const AstStatement* stmt,
		       YmslScope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kEnumDecl );

  ShString name = stmt->name();
  ymint n = stmt->enum_num();
  vector<pair<ShString, int> > elem_list(n);
  int next_val = 0;
  for (ymuint i = 0; i < n; ++ i) {
    const AstEnumConst* ec = stmt->enum_const(i);
    ShString elem_name = ec->name()->str_val();
    int v;
    if ( ec->expr() != NULL ) {
      // v = eval(ec->expr());
      // v が重複していないかチェック
    }
    else {
      // while ( check(next_val) ) {
      //   ++ next_val;
      // }
      v = next_val;
      ++ next_val;
    }
    elem_list[i] = make_pair(elem_name, v);
  }

  YmslType* type = mTypeMgr.enum_type(name, elem_list);

  scope->add_type(type);
}

// @brief 関数の定義を行う．
// @param[in] stmt 文
//
// stmt は kFuncDecl でなければならない．
void
YmslCompiler::reg_func(const AstStatement* stmt,
		       YmslScope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kFuncDecl );

  ShString name = stmt->name();
  const AstType* asttype = stmt->type();
  const YmslType* type = resolve_type(asttype, scope);

  ymuint np = stmt->param_num();
  vector<const YmslType*> input_type_list(np);
  for (ymuint i = 0; i < np; ++ i) {
    const AstParam* param = stmt->param(i);
    const AstType* asttype = param->type();
    const YmslType* type = resolve_type(asttype, scope);
    input_type_list[i] = type;
  }

  YmslFunction* func = new_function(name, type, input_type_list);
  scope->add_function(func);
}

// @brief 変数の定義を行う．
// @param[in] stmt 文
//
// stmt は kVarDecl でなければならない．
void
YmslCompiler::reg_var(const AstStatement* stmt,
		      YmslScope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kVarDecl );

  ShString name = stmt->name();
  const AstType* asttype = stmt->type();
  const YmslType* type = resolve_type(asttype, scope);

  YmslVar* var = new_var(name, type);
  scope->add_var(var);
}

// @brief 型の参照を解決する．
// @param[in] asttype 型を表す構文木
//
// 解決できない時には NULL を返す．
const YmslType*
YmslCompiler::resolve_type(const AstType* asttype,
			   YmslScope* scope)
{
  if ( asttype->named_type() ) {
    // スコープから名前の解決を行う
    return NULL;
  }

  switch ( asttype->type_id() ) {
  case kVoidType:
    return mTypeMgr.void_type();

  case kBooleanType:
    return mTypeMgr.boolean_type();

  case kIntType:
    return mTypeMgr.int_type();

  case kFloatType:
    return mTypeMgr.float_type();

  case kStringType:
    return mTypeMgr.string_type();

  case kArrayType:
    {
      const YmslType* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.array_type(elem_type);
    }
    break;

  case kSetType:
    {
      const YmslType* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.set_type(elem_type);
    }
    break;

  case kMapType:
    {
      const YmslType* key_type = resolve_type(asttype->key_type(), scope);
      if ( key_type == NULL ) {
	break;
      }
      const YmslType* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.map_type(key_type, elem_type);
    }
    break;

  case kFuncType:
  case kClassType:
  case kEnumType:
  case kUserDefType:
    // AstType でこれらの型はありえない．
    ASSERT_NOT_REACHED;
    break;

  }
  return NULL;
}

// @brief 参照の解決を行う．
// @param[in] stmt 文
// @param[in] scope 現在のスコープ
void
YmslCompiler::phase2(const AstStatement* stmt,
		     YmslScope* scope)
{
  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    break;

  case kBreak:
    break;

  case kContinue:
    break;

  case kDoWhile:
    elab_expr(stmt->expr(), scope);
    break;

  case kEnumDecl:
    //reg_enum(stmt, scope);
    break;

  case kEqAssign:
  case kEqPlus:
  case kEqMinus:
  case kEqMult:
  case kEqDiv:
  case kEqMod:
  case kEqLshift:
  case kEqRshift:
  case kEqAnd:
  case kEqOr:
  case kEqXor:
  case kExprStmt:
    elab_expr(stmt->lhs_expr(), scope);
    elab_expr(stmt->expr(), scope);
    break;

  case kFuncDecl:
    //reg_func(stmt, scope);
    break;

  case kGoto:
    break;

  case kIf:
    elab_expr(stmt->expr(), scope);
    break;

  case kImport:
    break;

  case kLabel:
    break;

  case kReturn:
    if ( stmt->expr() ) {
      elab_expr(stmt->expr(), scope);
    }
    break;

  case kSwitch:
    elab_expr(stmt->expr(), scope);
    break;

  case kToplevel:
    break;

  case kVarDecl:
    //reg_var(stmt, scope);
    if ( stmt->expr() ) {
      elab_expr(stmt->expr(), scope);
    }
    break;

  case kFor:
    {
      //YmslScope* for_scope = new YmslScope(scope);
      //phase1(stmt->init_stmt(), for_scope);
      //phase1(stmt->next_stmt(), for_scope);
      //phase1(stmt->stmt(), for_scope);
    }
    break;

  case kWhile:
    elab_expr(stmt->expr(), scope);
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}

// @brief 式の実体化を行う．
// @param[in] ast_expr 式を表す構文木
// @param[in] scope 現在のスコープ
YmslExpr*
YmslCompiler::elab_expr(const AstExpr* ast_expr,
			YmslScope* scope)
{
  YmslExpr* opr[3] = { NULL, NULL, NULL };
  for (ymuint i = 0; i < ast_expr->operand_num(); ++ i) {
    YmslExpr* opr1 = elab_expr(ast_expr->operand(i), scope);
    if ( opr1 == NULL ) {
      return NULL;
    }
    opr[i] = opr1;
    if ( i == 2 ) {
      break;
    }
  }

  switch ( ast_expr->expr_type() ) {
  case kTrue:
    return new YmslTrue();

  case kFalse:
    return new YmslFalse();

  case kIntConst:
    return new YmslIntConst(ast_expr->int_val());

  case kFloatConst:
    return new YmslFloatConst(ast_expr->float_val());

  case kStringConst:
    return new YmslStringConst(ast_expr->string_val());

  case kSymbolExpr:
#if 0
    {
      const AstSymbol* symbol = ast_leaf->symbol();
      ObjHandle* handle = scope->find(symbol->str_val());
      if ( handle == NULL ) {
	// symbol not found
	return NULL;
      }
      YmslVar* var = handle->var();
      if ( var == NULL ) {
	// symbol is not a variable
	return NULL;
      }
      return new YmslVarExpr(var);
    }
#endif
    break;

  case kArrayRef:
    {
      YmslExpr* body = elab_expr(ast_expr->body(), scope);
      YmslExpr* index = elab_expr(ast_expr->index(), scope);
      return new YmslArrayRef(body, index);
    }
    break;

  case kMemberRef:
    {
      YmslExpr* body = elab_expr(ast_expr->body(), scope);
      const AstSymbol* symbol = ast_expr->symbol();
      //return new YmslMemberRef(body, symbol);
    }
    break;

  case kCastInt:
    return new YmslOpExpr(kCastInt, opr[0]);

  case kCastBoolean:
    return new YmslOpExpr(kCastBoolean, opr[0]);

  case kCastFloat:
    return new YmslOpExpr(kCastFloat, opr[0]);

  case kBitNeg:
    return new YmslOpExpr(kBitNeg, opr[0]);

  case kLogNot:
    return new YmslOpExpr(kLogNot, opr[0]);

  case kUniPlus:
    return new YmslOpExpr(kUniPlus, opr[0]);

  case kUniMinus:
    return new YmslOpExpr(kUniMinus, opr[0]);

  case kBitAnd:
    return new YmslOpExpr(kBitAnd, opr[0], opr[1]);

  case kBitOr:
    return new YmslOpExpr(kBitOr, opr[0], opr[1]);

  case kBitXor:
    return new YmslOpExpr(kBitXor, opr[0], opr[1]);

  case kLogAnd:
    return new YmslOpExpr(kLogAnd, opr[0], opr[1]);

  case kLogOr:
    return new YmslOpExpr(kLogOr, opr[0], opr[1]);

  case kPlus:
    return new YmslOpExpr(kPlus, opr[0], opr[1]);

  case kMinus:
    return new YmslOpExpr(kMinus, opr[0], opr[1]);

  case kMult:
    return new YmslOpExpr(kMult, opr[0], opr[1]);

  case kDiv:
    return new YmslOpExpr(kDiv, opr[0], opr[1]);

  case kMod:
    return new YmslOpExpr(kMod, opr[0], opr[1]);

  case kLshift:
    return new YmslOpExpr(kLshift, opr[0], opr[1]);

  case kRshift:
    return new YmslOpExpr(kRshift, opr[0], opr[1]);

  case kEqual:
    return new YmslOpExpr(kEqual, opr[0], opr[1]);

  case kNotEq:
    return new YmslOpExpr(kNotEq, opr[0], opr[1]);

  case kLt:
    return new YmslOpExpr(kLt, opr[0], opr[1]);

  case kLe:
    return new YmslOpExpr(kLe, opr[0], opr[1]);

  case kGt:
    return new YmslOpExpr(kGt, opr[0], opr[1]);

  case kGe:
    return new YmslOpExpr(kGe, opr[0], opr[1]);

  case kIte:
    return new YmslOpExpr(kIte, opr[0], opr[1], opr[2]);

  case kFuncCall:
#if 0
    {
      YmslLeaf* leaf = elab_leaf(ast_expr->func(), scope);
      ymuint n = ast_expr->arglist_num();
      vector<YmslExpr*> arglist(n);
      for (ymuint i = 0; i < n; ++ i) {
	arglist[i] = elab_expr(ast_expr->arglist_elem(i), scope);
      }
      return new YmslFuncCall(leaf, arglist);
    }
#endif
    break;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 変数を生成する．
// @param[in] name 名前
// @param[in] type 型
YmslVar*
YmslCompiler::new_var(ShString name,
		      const YmslType* type)
{
  ymuint index = mVarList.size();
  YmslVar* var = new YmslVar(name, type, index);
  mVarList.push_back(var);

  return var;
}

// @brief 関数を生成する．
// @param[in] name 名前
// @param[in] type 出力の型
// @param[in] input_type_list 入力の型のリスト
YmslFunction*
YmslCompiler::new_function(ShString name,
			   const YmslType* type,
			   const vector<const YmslType*>& input_type_list)
{
  ymuint index = mFuncList.size();
  YmslFunction* func = new YmslFunction(name, type, input_type_list, index);
  mFuncList.push_back(func);

  return func;
}

END_NAMESPACE_YM_YMSL
