
/// @file YmslCompiler.cc
/// @brief YmslCompiler の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslCompiler.h"

#include "AstExpr.h"
#include "AstMgr.h"
#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstType.h"

#include "Function.h"
#include "Var.h"
#include "Scope.h"
#include "Type.h"
#include "SymHandle.h"
#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslCompiler
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
YmslCompiler::YmslCompiler() :
  mIrMgr(mTypeMgr)
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

  phase1(toplevel, NULL);

  for (vector<pair<const AstStatement*, Scope*> >::iterator p = mStmtList.begin();
       p != mStmtList.end(); ++ p) {
    const AstStatement* stmt = p->first;
    Scope* scope = p->second;
    phase2(stmt, scope);
  }

  for (vector<Scope*>::iterator p = mScopeList.begin();
      p != mScopeList.end(); ++ p) {
    delete *p;
  }
  mScopeList.clear();

  for (vector<Var*>::iterator p = mVarList.begin();
       p != mVarList.end(); ++ p) {
    delete *p;
  }
  mVarList.clear();

  for (vector<Function*>::iterator p = mFuncList.begin();
       p != mFuncList.end(); ++ p) {
    delete *p;
  }
  mFuncList.clear();

  mTypeMgr.clear();

  return true;
}

// @brief 要素の生成を行う．
// @param[in] stmt 文
void
YmslCompiler::phase1(const AstStatement* stmt,
		     Scope* scope)
{
  mStmtList.push_back(make_pair(stmt, scope));

  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    {
      Scope* block_scope = new_scope(scope);

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
    {
      ymuint n = stmt->switch_num();
      for (ymuint i = 0; i < n; ++ i) {
	phase1(stmt->case_stmt(i), scope);
      }
    }
    break;

  case kToplevel:
    {
      Scope* toplevel_scope = new_scope(NULL, ShString("__main__"));
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	phase1(stmt1, toplevel_scope);
      }
    }
    break;

  case kVarDecl:
    reg_var(stmt, scope);
    break;

  case kFor:
    {
      Scope* for_scope = new_scope(scope);
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
		       Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kEnumDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  SymHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return;
  }

  ymint n = stmt->enum_num();
  vector<pair<ShString, int> > elem_list(n);
  int next_val = 0;
  for (ymuint i = 0; i < n; ++ i) {
    const AstSymbol* ec_symbol = stmt->enum_const(i);
    ShString elem_name = ec_symbol->str_val();
    const AstExpr* ec_expr = stmt->enum_const_expr(i);
    int v;
    if ( ec_expr != NULL ) {
      // ec_expr が定数式であるかのチェック
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

  const Type* type = mTypeMgr.enum_type(name, elem_list);
  Scope* enum_scope = new_scope(scope, name);
  for (ymuint i = 0; i < n; ++ i) {
    ShString name = type->enum_elem_name(i);
    int val = type->enum_elem_val(i);
    IrNode* const_node = mIrMgr.new_IntConst(val);
    enum_scope->add_const(name, const_node);
  }
  scope->add_type(type, enum_scope);
}

// @brief 関数の定義を行う．
// @param[in] stmt 文
//
// stmt は kFuncDecl でなければならない．
void
YmslCompiler::reg_func(const AstStatement* stmt,
		       Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kFuncDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  SymHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return;
  }

  const AstType* asttype = stmt->type();
  const Type* output_type = resolve_type(asttype, scope);
  if ( output_type == NULL ) {
    // エラーメッセージをどこで出すか考える．
    return;
  }

  ymuint np = stmt->param_num();
  vector<const Type*> input_type_list(np);
  for (ymuint i = 0; i < np; ++ i) {
    const AstType* asttype = stmt->param_type(i);
    const Type* type = resolve_type(asttype, scope);
    if ( type == NULL ) {
      // エラーメッセージをどこで出すか考える．
      return;
    }
    input_type_list[i] = type;
  }

  const Type* ftype = mTypeMgr.function_type(output_type, input_type_list);

  Function* func = new_function(name, ftype);
  scope->add_function(func);
}

// @brief 変数の定義を行う．
// @param[in] stmt 文
//
// stmt は kVarDecl でなければならない．
void
YmslCompiler::reg_var(const AstStatement* stmt,
		      Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kVarDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  SymHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return;
  }

  const AstType* asttype = stmt->type();
  const Type* type = resolve_type(asttype, scope);
  if ( type == NULL ) {
    // エラーメッセージをどこで出すか考える．
    return;
  }

  Var* var = new_var(name, type);
  scope->add_var(var);
}

// @brief 型の参照を解決する．
// @param[in] asttype 型を表す構文木
//
// 解決できない時には NULL を返す．
const Type*
YmslCompiler::resolve_type(const AstType* asttype,
			   Scope* scope)
{
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
      const Type* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.array_type(elem_type);
    }
    break;

  case kSetType:
    {
      const Type* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.set_type(elem_type);
    }
    break;

  case kMapType:
    {
      const Type* key_type = resolve_type(asttype->key_type(), scope);
      if ( key_type == NULL ) {
	break;
      }
      const Type* elem_type = resolve_type(asttype->elem_type(), scope);
      if ( elem_type == NULL ) {
	break;
      }
      return mTypeMgr.map_type(key_type, elem_type);
    }
    break;

  case kNamedType:
    {
      // スコープから名前の解決を行う
      ymuint n = asttype->scope_num();
      Scope* cur_scope = scope;
      for (ymuint i = 0; i < n; ++ i) {
	const AstSymbol* scope1_symbol = asttype->scope(i);
	ShString scope1_name = scope1_symbol->str_val();
	SymHandle* h = cur_scope->find(scope1_name);
	if ( h == NULL ) {
	  // scope1_name is not found
	  return NULL;
	}
	cur_scope = h->scope();
	if ( cur_scope == NULL ) {
	  // scope1_name is not a scope
	  return NULL;
	}
      }
      const AstSymbol* name_symbol = asttype->name();
      ShString name = name_symbol->str_val();
      SymHandle* h = cur_scope->find(name);
      if ( h == NULL ) {
	// name is not defined
	return NULL;
      }
      const Type* type = h->named_type();
      if ( type == NULL ) {
	// name is not a type;
      }
      return type;
    }
    break;

  case kFuncType:
  case kEnumType:
  case kClassType:
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
		     Scope* scope)
{
  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    // 中身の statement は個別に phase2() で呼ばれる．
    break;

  case kBreak:
    break;

  case kContinue:
    break;

  case kDoWhile:
    //elab_expr(stmt->expr(), scope);
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
    //elab_expr(stmt->lhs_expr(), scope);
    //elab_expr(stmt->expr(), scope);
    break;

  case kFuncDecl:
    //reg_func(stmt, scope);
    break;

  case kGoto:
    break;

  case kIf:
    //elab_expr(stmt->expr(), scope);
    break;

  case kImport:
    break;

  case kLabel:
    break;

  case kReturn:
    if ( stmt->expr() ) {
      //elab_expr(stmt->expr(), scope);
    }
    break;

  case kSwitch:
    //elab_expr(stmt->expr(), scope);
    break;

  case kToplevel:
    break;

  case kVarDecl:
    //reg_var(stmt, scope);
    if ( stmt->expr() ) {
      //elab_expr(stmt->expr(), scope);
    }
    break;

  case kFor:
    {
      //Scope* for_scope = new Scope(scope);
      //phase1(stmt->init_stmt(), for_scope);
      //phase1(stmt->next_stmt(), for_scope);
      //phase1(stmt->stmt(), for_scope);
    }
    break;

  case kWhile:
    //elab_expr(stmt->expr(), scope);
    break;

  default:
    ASSERT_NOT_REACHED;
  }
}

// @brief 右辺式の実体化を行う．
// @param[in] ast_expr 式を表す構文木
// @param[in] scope 現在のスコープ
IrNode*
YmslCompiler::elab_rhs(const AstExpr* ast_expr,
		       Scope* scope)
{
  ASSERT_COND( ast_expr->operand_num() <= 3 );

  IrNode* op0 = NULL;
  IrNode* op1 = NULL;
  IrNode* op2 = NULL;
  ymuint nop = ast_expr->operand_num();
  if ( nop > 0 ) {
    op0 = elab_rhs(ast_expr->operand(0), scope);
    if ( op0 == NULL ) {
      return NULL;
    }
    if ( nop > 1 ) {
      op1 = elab_rhs(ast_expr->operand(1), scope);
      if ( op1 == NULL ) {
	return NULL;
      }
      if ( nop > 2 ) {
	op2 = elab_rhs(ast_expr->operand(2), scope);
	if ( op2 == NULL ) {
	  return NULL;
	}
      }
    }
  }

  OpCode opcode = kOpHalt;
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

  case kCastInt:
    opcode = kOpCastInt;
    break;

  case kCastBoolean:
    opcode = kOpCastBoolean;
    break;

  case kCastFloat:
    opcode = kOpCastFloat;
    break;

  case kBitNeg:
    opcode = kOpBitNeg;
    break;

  case kLogNot:
    opcode = kOpLogNot;
    break;

  case kUniMinus:
    opcode = kOpUniMinus;
    break;

  case kBitAnd:
    opcode = kOpBitAnd;
    break;

  case kBitOr:
    opcode = kOpBitOr;
    break;

  case kBitXor:
    opcode = kOpBitXor;
    break;

  case kLogAnd:
    opcode = kOpLogAnd;
    break;

  case kLogOr:
    opcode = kOpLogOr;
    break;

  case kPlus:
    opcode = kOpPlus;
    break;

  case kMinus:
    opcode = kOpMinus;
    break;

  case kMult:
    opcode = kOpMult;
    break;

  case kDiv:
    opcode = kOpDiv;
    break;

  case kMod:
    opcode = kOpMod;
    break;

  case kLshift:
    opcode = kOpLshift;
    break;

  case kRshift:
    opcode = kOpRshift;
    break;

  case kEqual:
    opcode = kOpEqual;
    break;

  case kNotEq:
    opcode = kOpNotEq;
    break;

  case kLt:
    opcode = kOpLt;
    break;

  case kLe:
    opcode = kOpLe;
    break;

  case kGt:
    opcode = kOpLt;
    {
      IrNode* tmp = op0;
      op0 = op1;
      op1 = tmp;
    }
    break;

  case kGe:
    opcode = kOpLe;
    {
      IrNode* tmp = op0;
      op0 = op1;
      op1 = tmp;
    }
    break;

  case kIte:
    opcode = kOpIte;
    break;

  case kFuncCall:
    {
      const AstExpr* func_expr = ast_expr->func();
      const Function* func = NULL;
      if ( func_expr->expr_type() == kSymbolExpr ) {
	SymHandle* h = scope->find(func_expr->symbol()->str_val());
	if ( h == NULL ) {
	  // h is not found
	  return NULL;
	}
	func = h->function();
	if ( func == NULL ) {
	  // func is not a function
	  return NULL;
	}
      }
      else {
	// 未完
      }
      ymuint n = ast_expr->arglist_num();
      vector<IrNode*> arglist(n);
      for (ymuint i = 0; i < n; ++ i) {
	IrNode* arg = elab_rhs(ast_expr->arglist_elem(i), scope);
	// arg->type() と func->type()->function_input_type(i) をチェック
	// 必要に応じてキャストノードを挿入する．
	arglist[i] = arg;
      }
      return mIrMgr.new_FuncCall(func, arglist);
    }
    break;

  default:
    return elab_rhs_primary(ast_expr, scope);
  }

  if ( nop == 1 ) {
    const Type* op0_type = op0->type();
    const Type* op0_rtype;
    const Type* type = mTypeMgr.calc_type1(opcode, op0_type, op0_rtype);
    if ( type == NULL ) {
      // type mismatch
      return NULL;
    }

    if ( op0_type != op0_rtype ) {
      // キャストノードの挿入
      // 結果を op0 に代入
    }
    return mIrMgr.new_UniOp(opcode, type, op0);
  }
  else if ( nop == 2 ) {
    const Type* op0_type = op0->type();
    const Type* op1_type = op1->type();
    const Type* op0_rtype;
    const Type* op1_rtype;
    const Type* type = mTypeMgr.calc_type2(opcode, op0_type, op1_type, op0_rtype, op1_rtype);
    if ( type == NULL ) {
      // type mismatch
      return NULL;
    }

    if ( op0_rtype != op0_type ) {
      // キャストノードの挿入
    }

    if ( op1_rtype != op1_type ) {
      // キャストノードの挿入
    }
    return mIrMgr.new_BinOp(opcode, type, op0, op1);
  }
  else if ( nop == 3 ) {
    const Type* op0_type = op0->type();
    const Type* op1_type = op1->type();
    const Type* op2_type = op2->type();
    const Type* op0_rtype;
    const Type* op1_rtype;
    const Type* op2_rtype;
    const Type* type = mTypeMgr.calc_type3(opcode, op0_type, op1_type, op2_type, op0_rtype, op1_rtype, op2_rtype);
    if ( type == NULL ) {
      // type mismatch
      return NULL;
    }

    if ( op0_rtype != op0_type ) {
      // キャストノードの挿入
    }

    if ( op1_rtype != op1_type ) {
      // キャストノードの挿入
    }

    if ( op2_rtype != op2_type ) {
      // キャストノードの挿入
    }
    return mIrMgr.new_TriOp(opcode, type, op0, op1, op2);
  }

  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 右辺式の実体化を行う．(プライマリ用)
// @param[in] ast_expr 式を表す構文木
// @param[in] scope 現在のスコープ
IrNode*
YmslCompiler::elab_rhs_primary(const AstExpr* ast_expr,
			       Scope* scope)
{
  SymHandle* h = resolve_symbol(ast_expr, scope);
  if ( h != NULL ) {
    const Var* var = h->var();
    if ( var != NULL ) {
      return mIrMgr.new_VarRef(var);
    }
    IrNode* const_node = h->const_node();
    if ( const_node != NULL ) {
      return const_node;
    }
  }

  switch ( ast_expr->expr_type() ) {
  case kSymbolExpr:
    // resolve_symbol() で失敗しているので
    // ここでは成功するはずがない．
    // symbol not found
    return NULL;

  case kArrayRef:
    {
      IrNode* body = elab_rhs_primary(ast_expr->body(), scope);
      IrNode* index = elab_rhs(ast_expr->index(), scope);
      // ...
    }
    break;

  case kMemberRef:
    {
      IrNode* body = elab_rhs_primary(ast_expr->body(), scope);
    }
    break;

  default:
    break;
  }
  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 左辺式の実体化を行う．
// @param[in] ast_expr 式を表す構文木
// @param[in] scope 現在のスコープ
// @param[out] base ベース値
// @param[out] offset オフセット
//
// エラーが起きたら false を返す．
// 書き込む位置が決まっている場合には
// base = NULL とする．
bool
YmslCompiler::elab_lhs(const AstExpr* ast_expr,
		       Scope* scope,
		       IrNode& base,
		       int& offset)
{
  switch ( ast_expr->expr_type() ) {
  case kSymbolExpr:
    break;

  case kArrayRef:
    break;

  case kMemberRef:
    break;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return false;
}

// @brief 式からシンボルの解決を行う．
// @param[in] expr 式
// @param[in] scopde 現在のスコープ
SymHandle*
YmslCompiler::resolve_symbol(const AstExpr* expr,
			     Scope* scope)
{
  switch ( expr->expr_type() ) {
  case kSymbolExpr:
    return scope->find(expr->symbol()->str_val());

  case kMemberRef:
    {
      const AstExpr* body = expr->body();
      SymHandle* h = resolve_symbol(body, scope);
      Scope* scope1 = h->scope();
      if ( scope1 == NULL ) {
	// not scope
	return NULL;
      }
      return scope1->find(expr->symbol()->str_val());
    }
    break;

  default:
    break;
  }

  return NULL;
}

// @brief スコープを生成する．
// @param[in] parent_scope 親のスコープ
// @param[in] name スコープ名
Scope*
YmslCompiler::new_scope(Scope* parent_scope,
			ShString name)
{
  Scope* scope = new Scope(parent_scope, name);
  mScopeList.push_back(scope);

  return scope;
}

// @brief 変数を生成する．
// @param[in] name 名前
// @param[in] type 型
Var*
YmslCompiler::new_var(ShString name,
		      const Type* type)
{
  ymuint index = mVarList.size();
  Var* var = new Var(name, type, index);
  mVarList.push_back(var);

  return var;
}

// @brief 関数を生成する．
// @param[in] name 名前
// @param[in] type 出力の型
// @param[in] input_type_list 入力の型のリスト
Function*
YmslCompiler::new_function(ShString name,
			   const Type* type)
{
  ymuint index = mFuncList.size();
  Function* func = new Function(name, type, index);
  mFuncList.push_back(func);

  return func;
}

END_NAMESPACE_YM_YMSL
