
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCompiler.h"

#include "AstExpr.h"
#include "AstMgr.h"
#include "AstParam.h"
#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstType.h"

#include "IrFunction.h"
#include "IrVar.h"
#include "IrScope.h"
#include "IrType.h"
#include "IrExpr.h"
#include "IrHandle.h"


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
  AstMgr ast_mgr;

  bool stat = ast_mgr.read_source(ido);
  if ( !stat ) {
    return false;
  }

  AstStatement* toplevel = ast_mgr.toplevel();

  mStmtList.clear();

  IrScope* toplevel_scope = mIrMgr.new_Scope(NULL, ShString("__main__"));
  phase1(toplevel, toplevel_scope);

  for (vector<pair<const AstStatement*, IrScope*> >::iterator p = mStmtList.begin();
       p != mStmtList.end(); ++ p) {
    const AstStatement* stmt = p->first;
    IrScope* scope = p->second;
    phase2(stmt, scope);
  }

  return true;
}

// @brief 要素の生成を行う．
// @param[in] stmt 文
void
YmslCompiler::phase1(const AstStatement* stmt,
		     IrScope* scope)
{
  mStmtList.push_back(make_pair(stmt, scope));

  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    {
      IrScope* block_scope = mIrMgr.new_Scope(scope);

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
      IrScope* for_scope = mIrMgr.new_Scope(scope);
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
		       IrScope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kEnumDecl );

  ShString name = stmt->name();
  ymint n = stmt->enum_num();
  vector<pair<ShString, int> > elem_list(n);
  int next_val = 0;
  for (ymuint i = 0; i < n; ++ i) {
    const AstSymbol* ec_symbol = stmt->enum_const(i);
    ShString elem_name = ec_symbol->str_val();
    const AstExpr* ec_expr = stmt->enum_const_expr(i);
    int v;
    if ( ec_expr != NULL ) {
      // v = eval(ec_expr());
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

  IrType* type = mTypeMgr.enum_type(name, elem_list);

  scope->add_type(type);
}

// @brief 関数の定義を行う．
// @param[in] stmt 文
//
// stmt は kFuncDecl でなければならない．
void
YmslCompiler::reg_func(const AstStatement* stmt,
		       IrScope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kFuncDecl );

  ShString name = stmt->name();
  const AstType* asttype = stmt->type();
  const IrType* type = resolve_type(asttype, scope);

  ymuint np = stmt->param_num();
  vector<const IrType*> input_type_list(np);
  for (ymuint i = 0; i < np; ++ i) {
    const AstParam* param = stmt->param(i);
    const AstType* asttype = param->type();
    const IrType* type = resolve_type(asttype, scope);
    input_type_list[i] = type;
  }

  IrFunction* func = new_function(name, type, input_type_list);
  scope->add_function(func);
}

// @brief 変数の定義を行う．
// @param[in] stmt 文
//
// stmt は kVarDecl でなければならない．
void
YmslCompiler::reg_var(const AstStatement* stmt,
		      IrScope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kVarDecl );

  ShString name = stmt->name();
  const AstType* asttype = stmt->type();
  const IrType* type = resolve_type(asttype, scope);

  IrVar* var = new_var(name, type);
  scope->add_var(var);
}

// @brief 型の参照を解決する．
// @param[in] asttype 型を表す構文木
//
// 解決できない時には NULL を返す．
const IrType*
YmslCompiler::resolve_type(const AstType* asttype,
			   IrScope* scope)
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
      const IrType* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.array_type(elem_type);
    }
    break;

  case kSetType:
    {
      const IrType* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.set_type(elem_type);
    }
    break;

  case kMapType:
    {
      const IrType* key_type = resolve_type(asttype->key_type(), scope);
      if ( key_type == NULL ) {
	break;
      }
      const IrType* elem_type = resolve_type(asttype->elem_type(), scope);
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
		     IrScope* scope)
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
      //IrScope* for_scope = mIrMgr.new_Scope(scope);
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
IrExpr*
YmslCompiler::elab_expr(const AstExpr* ast_expr,
			IrScope* scope)
{
  IrExpr* opr[3] = { NULL, NULL, NULL };
  for (ymuint i = 0; i < ast_expr->operand_num(); ++ i) {
    IrExpr* opr1 = elab_expr(ast_expr->operand(i), scope);
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
    return mIrMgr.new_True();

  case kFalse:
    return mIrMgr.new_False();

  case kIntConst:
    return mIrMgr.new_IntConst(ast_expr->int_val());

  case kFloatConst:
    return mIrMgr.new_FloatConst(ast_expr->float_val());

  case kStringConst:
    return mIrMgr.new_StringConst(ast_expr->string_val());

  case kSymbolExpr:
    return symbol2expr(ast_expr->symbol(), scope);

  case kArrayRef:
    {
      IrExpr* body = elab_expr(ast_expr->body(), scope);
      IrExpr* index = elab_expr(ast_expr->index(), scope);
      return mIrMgr.new_ArrayRef(body, index);
    }
    break;

  case kMemberRef:
    {
      IrExpr* body = elab_expr(ast_expr->body(), scope);
      const AstSymbol* symbol = ast_expr->symbol();
      switch ( body->expr_type() ) {
      case kScopeExpr:
	return symbol2expr(symbol, body->scope());

      case kEnumExpr:
	{
	  const IrType* type = body->enum_type();
	  int index = type->enum_index(symbol->str_val());
	  if ( index == -1 ) {
	    // symbol not found
	    return NULL;
	  }

	  int val = type->enum_elem_val(index);
	  return mIrMgr.new_IntConst(val);
	}
	break;

      case kSymbolExpr:
	{
	  const IrVar* var = body->var();
	  const IrType* type = var->value_type();
	  // type から symbol という名のメンバを探す．
	  //return mIrMgr.new_MemberRef(body, symbol);
	}
	break;

      default:
	break;
      }
      ASSERT_NOT_REACHED;
      return NULL;
    }
    break;

  case kCastInt:
    return mIrMgr.new_OpExpr(kCastInt, opr[0]);

  case kCastBoolean:
    return mIrMgr.new_OpExpr(kCastBoolean, opr[0]);

  case kCastFloat:
    return mIrMgr.new_OpExpr(kCastFloat, opr[0]);

  case kBitNeg:
    return mIrMgr.new_OpExpr(kBitNeg, opr[0]);

  case kLogNot:
    return mIrMgr.new_OpExpr(kLogNot, opr[0]);

  case kUniPlus:
    return mIrMgr.new_OpExpr(kUniPlus, opr[0]);

  case kUniMinus:
    return mIrMgr.new_OpExpr(kUniMinus, opr[0]);

  case kBitAnd:
    return mIrMgr.new_OpExpr(kBitAnd, opr[0], opr[1]);

  case kBitOr:
    return mIrMgr.new_OpExpr(kBitOr, opr[0], opr[1]);

  case kBitXor:
    return mIrMgr.new_OpExpr(kBitXor, opr[0], opr[1]);

  case kLogAnd:
    return mIrMgr.new_OpExpr(kLogAnd, opr[0], opr[1]);

  case kLogOr:
    return mIrMgr.new_OpExpr(kLogOr, opr[0], opr[1]);

  case kPlus:
    return mIrMgr.new_OpExpr(kPlus, opr[0], opr[1]);

  case kMinus:
    return mIrMgr.new_OpExpr(kMinus, opr[0], opr[1]);

  case kMult:
    return mIrMgr.new_OpExpr(kMult, opr[0], opr[1]);

  case kDiv:
    return mIrMgr.new_OpExpr(kDiv, opr[0], opr[1]);

  case kMod:
    return mIrMgr.new_OpExpr(kMod, opr[0], opr[1]);

  case kLshift:
    return mIrMgr.new_OpExpr(kLshift, opr[0], opr[1]);

  case kRshift:
    return mIrMgr.new_OpExpr(kRshift, opr[0], opr[1]);

  case kEqual:
    return mIrMgr.new_OpExpr(kEqual, opr[0], opr[1]);

  case kNotEq:
    return mIrMgr.new_OpExpr(kNotEq, opr[0], opr[1]);

  case kLt:
    return mIrMgr.new_OpExpr(kLt, opr[0], opr[1]);

  case kLe:
    return mIrMgr.new_OpExpr(kLe, opr[0], opr[1]);

  case kGt:
    return mIrMgr.new_OpExpr(kGt, opr[0], opr[1]);

  case kGe:
    return mIrMgr.new_OpExpr(kGe, opr[0], opr[1]);

  case kIte:
    return mIrMgr.new_OpExpr(kIte, opr[0], opr[1], opr[2]);

  case kFuncCall:
    {
      IrExpr* func_expr = elab_expr(ast_expr->func(), scope);
      ymuint n = ast_expr->arglist_num();
      vector<IrExpr*> arglist(n);
      for (ymuint i = 0; i < n; ++ i) {
	arglist[i] = elab_expr(ast_expr->arglist_elem(i), scope);
      }
      return mIrMgr.new_FuncCall(func_expr, arglist);
    }
    break;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief シンボルに対応する式を返す．
// @param[in] symbol シンボル
// @param[in] scope スコープ
IrExpr*
YmslCompiler::symbol2expr(const AstSymbol* symbol,
			  IrScope* scope)
{
  IrHandle* handle = scope->find(symbol->str_val());
  if ( handle == NULL ) {
    // symbol not found
    return NULL;
  }

  IrFunction* func = handle->function();
  if ( func != NULL ) {
    return mIrMgr.new_FuncExpr(func);
  }

  IrVar* var = handle->var();
  if ( var != NULL ) {
    return mIrMgr.new_VarExpr(var);
  }

  IrScope* scope1 = handle->scope();
  if ( scope1 != NULL ) {
    return mIrMgr.new_ScopeExpr(scope1);
  }

  const IrType* type = handle->named_type();
  if ( type != NULL ) {
    return mIrMgr.new_EnumExpr(type);
  }

  // 型が合わない．
  return NULL;
}

// @brief 変数を生成する．
// @param[in] name 名前
// @param[in] type 型
IrVar*
YmslCompiler::new_var(ShString name,
		      const IrType* type)
{
  ymuint index = mVarList.size();
  IrVar* var = mIrMgr.new_Var(name, type, index);
  mVarList.push_back(var);

  return var;
}

// @brief 関数を生成する．
// @param[in] name 名前
// @param[in] type 出力の型
// @param[in] input_type_list 入力の型のリスト
IrFunction*
YmslCompiler::new_function(ShString name,
			   const IrType* type,
			   const vector<const IrType*>& input_type_list)
{
  ymuint index = mFuncList.size();
  IrFunction* func = mIrMgr.new_Function(name, type, input_type_list, index);
  mFuncList.push_back(func);

  return func;
}

END_NAMESPACE_YM_YMSL
