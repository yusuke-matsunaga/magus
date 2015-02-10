
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"
#include "TypeMgr.h"

#include "AstExpr.h"
#include "AstMgr.h"
#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstType.h"

#include "Function.h"
#include "Var.h"
#include "Scope.h"
#include "Type.h"
#include "IrHandle.h"
#include "IrNode.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
IrMgr::IrMgr()
{
}

// @brief デストラクタ
IrMgr::~IrMgr()
{
  clear();
}

// @brief クリアする．
void
IrMgr::clear()
{
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

  mUndefList.clear();

  mTypeMgr.clear();

  mAlloc.destroy();
}

// @brief 抽象構文木から中間表現を生成する．
// @param[in] ast_root 抽象構文木の根のノード
//
// エラーが起きたら false を返す．
bool
IrMgr::elaborate(const AstStatement* ast_root,
		 vector<IrNode*>& node_list)
{
  ASSERT_COND( ast_root->stmt_type() == kToplevel );

  mFuncCallList.clear();
  mUndefList.clear();

  // 中間表現を作る．
  Scope* toplevel_scope = new_scope(NULL, ShString("__main__"));
  elab_stmt(ast_root, toplevel_scope, NULL, NULL, node_list);

  // 関数呼び出しの解決を行う．
  for (vector<FuncCallStub>::iterator p = mFuncCallList.begin();
       p != mFuncCallList.end(); ++ p) {
    const AstExpr* func_expr = p->mExpr;
    Scope* scope = p->mScope;
    IrNode* node = p->mNode;
    resolve_func(func_expr, scope, node);
  }

  // ラベルの解決が行われているかチェックする．
  for (vector<IrNode*>::iterator p = mUndefList.begin();
       p != mUndefList.end(); ++ p) {
    IrNode* label = *p;
    if ( !label->is_defined() ) {
      // undefined
      return false;
    }
  }

  return true;
}

// @brief 要素の生成を行う．
// @param[in] stmt 文
// @param[in] scope 現在のスコープ
// @param[in] start_label ブロックの開始ラベル
// @param[in] end_label ブロックの終了ラベル
// @param[out] node_list 生成されたノードのリスト
void
IrMgr::elab_stmt(const AstStatement* stmt,
		 Scope* scope,
		 IrNode* start_label,
		 IrNode* end_label,
		 vector<IrNode*>& node_list)
{
  switch ( stmt->stmt_type() ) {
  case kBlockStmt:
    {
      Scope* block_scope = new_scope(scope);
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	elab_stmt(stmt1, block_scope, start_label, end_label, node_list);
      }
    }
    break;

  case kBreak:
    {
      if ( end_label == NULL ) {
	// not inside loop
	return;
      }
      IrNode* node = new_Jump(kOpJump, end_label);
      node_list.push_back(node);
    }
    break;

  case kConstDecl:
    reg_const(stmt, scope);
    break;

  case kContinue:
    {
      if ( start_label == NULL ) {
	// not inside loop
	return;
      }
      IrNode* node = new_Jump(kOpJump, start_label);
      node_list.push_back(node);
    }
    break;

  case kDecr:
    {
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }
      // lhs_handle の指しているものが int 型の変数かチェック

      IrNode* node = new_InplaceUniOp(kOpDec, lhs_handle);
      node_list.push_back(node);
    }
    break;

  case kDoWhile:
    {
      IrNode* start1 = new_Label();
      IrNode* end1 = new_Label();
      node_list.push_back(start1);
      elab_stmt(stmt->stmt(), scope, start1, end1, node_list);
      IrNode* cond = elab_rhs(stmt->expr(), scope);
      IrNode* node1 = new_Jump(kOpBranchTrue, start1, cond);
      node_list.push_back(node1);
      node_list.push_back(end1);
    }
    break;

  case kEnumDecl:
    reg_enum(stmt, scope);
    break;

  case kEqAssign:
    {
      IrNode* rhs = elab_rhs(stmt->expr(), scope);
      if ( rhs == NULL ) {
	// エラー
	return;
      }
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }

      // lhs_handle->type() と rhs->type() のチェック
      // 必要ならキャスト
      IrNode* node = new_Store(lhs_handle, rhs);
      node_list.push_back(node);
    }
    break;

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
    {
      IrNode* rhs = elab_rhs(stmt->expr(), scope);
      if ( rhs == NULL ) {
	// エラー
	return;
      }
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }

      // lhs_handle->type() と rhs->type() のチェック
      // 必要ならキャスト
      OpCode opcode;
      switch ( stmt->stmt_type() ) {
      case kEqPlus:   opcode = kOpInplaceAdd; break;
      case kEqMinus:  opcode = kOpInplaceSub; break;
      case kEqMult:   opcode = kOpInplaceMul; break;
      case kEqDiv:    opcode = kOpInplaceDiv; break;
      case kEqMod:    opcode = kOpInplaceMod; break;
      case kEqLshift: opcode = kOpInplaceLshift; break;
      case kEqRshift: opcode = kOpInplaceRshift; break;
      case kEqAnd:    opcode = kOpInplaceBitAnd; break;
      case kEqOr:     opcode = kOpInplaceBitOr; break;
      case kEqXor:    opcode = kOpInplaceBitXor; break;
      default: ASSERT_NOT_REACHED; break;
      }
      IrNode* node = new_InplaceBinOp(opcode, lhs_handle, rhs);
      node_list.push_back(node);
    }
    break;

  case kExprStmt:
    {
      IrNode* node = elab_rhs(stmt->expr(), scope);
      node_list.push_back(node);
    }
    break;

  case kFor:
    {
      Scope* for_scope = new_scope(scope);
      elab_stmt(stmt->init_stmt(), for_scope, NULL, NULL, node_list);
      IrNode* start1 = new_Label();
      node_list.push_back(start1);
      IrNode* end1 = new_Label();
      IrNode* cond = elab_rhs(stmt->expr(), for_scope);
      IrNode* node1 = new_Jump(kOpBranchFalse, end1, cond);
      node_list.push_back(node1);
      elab_stmt(stmt->stmt(), for_scope, start1, end1, node_list);
      elab_stmt(stmt->next_stmt(), for_scope, start1, end1, node_list);
      IrNode* node2 = new_Jump(kOpJump, start1);
      node_list.push_back(node2);
      node_list.push_back(end1);
    }
    break;

  case kFuncDecl:
    reg_func(stmt, scope);
    break;

  case kGoto:
    {
      const AstSymbol* label_symbol = stmt->label();
      IrHandle* h = scope->find(label_symbol->str_val());
      IrNode* label_node;
      if ( h == NULL ) {
	// ラベルが未定義
	// ノードを作ってしまう．
	label_node = new_Label();
	h = new_LabelHandle(label_symbol->str_val(), label_node);
	scope->add(h);
	mUndefList.push_back(label_node);
      }
      else {
	if ( h->handle_type() != IrHandle::kLabel ) {
	  // label_symbol is not a label
	  return;
	}
      }
      IrNode* node = new_Jump(kOpJump, label_node);
      node_list.push_back(node);
    }
    break;

  case kIf:
    {
      IrNode* cond = elab_rhs(stmt->expr(), scope);
      IrNode* label1 = new_Label();
      IrNode* label2 = new_Label();
      IrNode* node1 = new_Jump(kOpBranchFalse, label1, cond);
      node_list.push_back(node1);
      elab_stmt(stmt->stmt(), scope, start_label, end_label, node_list);
      IrNode* node2 = new_Jump(kOpJump, label2);
      node_list.push_back(node2);
      node_list.push_back(label1);
      elab_stmt(stmt->else_stmt(), scope, start_label, end_label, node_list);
      node_list.push_back(label2);
    }
    break;

  case kImport:
    break;

  case kIncr:
    {
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }
      IrNode* node = new_InplaceUniOp(kOpInc, lhs_handle);
      node_list.push_back(node);
    }
    break;

  case kLabel:
    {
      const AstSymbol* label_symbol = stmt->label();
      IrHandle* h = scope->find(label_symbol->str_val());
      IrNode* label_node;
      if ( h == NULL ) {
	label_node = new_Label();
	h = new_LabelHandle(label_symbol->str_val(), label_node);
	scope->add(h);
      }
      else {
	if ( h->handle_type() != IrHandle::kLabel ) {
	  // duplicate definition
	  // というかラベルじゃないものだった．
	  return;
	}
	if ( label_node->is_defined() ) {
	  // 二重定義
	  return;
	}
      }
      label_node->set_defined();
      node_list.push_back(label_node);
    }
    break;

  case kNullStmt:
    break;

  case kReturn:
    {
      const AstExpr* expr = stmt->expr();
      IrNode* ret_val = NULL;
      if ( expr != NULL ) {
	ret_val = elab_rhs(expr, scope);
      }
      IrNode* node = new_Return(ret_val);
      node_list.push_back(node);
    }
    break;

  case kSwitch:
    {
      IrNode* cond = elab_rhs(stmt->expr(), scope);
      ymuint n = stmt->switch_num();
      for (ymuint i = 0; i < n; ++ i) {
	vector<IrNode*> node_list1;
	elab_stmt(stmt->case_stmt(i), scope, start_label, end_label, node_list1);
      }
    }
    break;

  case kToplevel:
    {
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	elab_stmt(stmt1, scope, NULL, NULL, node_list);
      }
    }
    break;

  case kVarDecl:
    reg_var(stmt, scope);
    break;

  case kWhile:
    {
      IrNode* start1 = new_Label();
      node_list.push_back(start1);
      IrNode* cond = elab_rhs(stmt->expr(), scope);
      IrNode* end1 = new_Label();
      IrNode* node1 = new_Jump(kOpBranchFalse, end1, cond);
      node_list.push_back(node1);
      elab_stmt(stmt->stmt(), scope, start1, end1, node_list);
      IrNode* node2 = new_Jump(kOpJump, start1);
      node_list.push_back(node2);
      node_list.push_back(end1);
    }
    break;

#if 0
  default:
    cerr << "stmt_type() = " << stmt->stmt_type() << endl;
    ASSERT_NOT_REACHED;
#endif
  }
}

// @brief enum 型の定義を行う．
// @param[in] stmt 文
//
// stmt は kEnumDecl でなければならない．
void
IrMgr::reg_enum(const AstStatement* stmt,
		Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kEnumDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
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
      IrNode* node = elab_rhs(ec_expr, scope);
      if ( node == NULL ) {
	// エラー
	return;
      }
      if ( !node->is_static() ) {
	// ec_expr が定数式ではない．
	return;
      }
      if ( node->type() != mTypeMgr.int_type() ) {
	// 整数型ではない．
	return;
      }
      // v = eval(node);
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
    IrNode* const_node = new_IntConst(val);
    IrHandle* h = new_ConstHandle(name, const_node);
    enum_scope->add(h);
  }

  IrHandle* h1 = new_TypeHandle(type, enum_scope);
  scope->add(h1);
}

// @brief 関数の定義を行う．
// @param[in] stmt 文
//
// stmt は kFuncDecl でなければならない．
void
IrMgr::reg_func(const AstStatement* stmt,
		Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kFuncDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
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
  IrHandle* h1 = new_FuncHandle(func);
  scope->add(h1);
}

// @brief 変数の定義を行う．
// @param[in] stmt 文
//
// stmt は kVarDecl でなければならない．
void
IrMgr::reg_var(const AstStatement* stmt,
	       Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kVarDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
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
  IrHandle* h1 = new_VarHandle(var);
  scope->add(h1);
}

// @brief 定数の定義を行う．
// @param[in] stmt 文
//
// stmt は kConstDecl でなければならない．
void
IrMgr::reg_const(const AstStatement* stmt,
		 Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == kConstDecl );

  const AstSymbol* name_symbol = stmt->name();
  ShString name = name_symbol->str_val();

  IrHandle* h = scope->find_local(name);
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

  IrNode* node = elab_rhs(stmt->expr(), scope);
  // node->type() が type と互換性があるかをチェック
  // node が定数式かチェック

  IrHandle* h1 = new_ConstHandle(name, node);
  scope->add(h1);
}

// @brief 型の参照を解決する．
// @param[in] asttype 型を表す構文木
//
// 解決できない時には NULL を返す．
const Type*
IrMgr::resolve_type(const AstType* asttype,
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
	IrHandle* h = cur_scope->find(scope1_name);
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
      IrHandle* h = cur_scope->find(name);
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

// @brief 右辺式の実体化を行う．
// @param[in] ast_expr 式を表す構文木
// @param[in] scope 現在のスコープ
IrNode*
IrMgr::elab_rhs(const AstExpr* ast_expr,
		Scope* scope)
{
  IrNode* op0 = NULL;
  IrNode* op1 = NULL;
  IrNode* op2 = NULL;
  ymuint nop = ast_expr->operand_num();
  ASSERT_COND( nop <= 3 );
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

  IrNode* node = NULL;
  OpCode opcode = kOpHalt;
  switch ( ast_expr->expr_type() ) {
  case kTrue:
    node = new_True();
    return node;

  case kFalse:
    node = new_False();
    return node;

  case kIntConst:
    node = new_IntConst(ast_expr->int_val());
    return node;

  case kFloatConst:
    node = new_FloatConst(ast_expr->float_val());
    return node;

  case kStringConst:
    node = new_StringConst(ast_expr->string_val());
    return node;

  case kSymbolExpr:
  case kArrayRef:
  case kMemberRef:
    {
      IrHandle* h = elab_primary(ast_expr, scope);
      if ( h == NULL ) {
	return NULL;
      }
      return new_Load(h);
    }

  case kFuncCall:
    {
      const AstExpr* func_expr = ast_expr->func();
      ymuint n = ast_expr->arglist_num();
      vector<IrNode*> arglist(n);
      for (ymuint i = 0; i < n; ++ i) {
	IrNode* arg = elab_rhs(ast_expr->arglist_elem(i), scope);
	// arg->type() と func->type()->function_input_type(i) をチェック
	// 必要に応じてキャストノードを挿入する．
	arglist[i] = arg;
      }
      node = new_FuncCall(NULL, arglist);
      mFuncCallList.push_back(FuncCallStub(func_expr, scope, node));
      return node;
    }
    break;

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
    opcode = kOpAdd;
    break;

  case kMinus:
    opcode = kOpSub;
    break;

  case kMult:
    opcode = kOpMul;
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
    node = new_UniOp(opcode, type, op0);
    return node;
  }
  else if ( nop == 2 ) {
    const Type* op0_type = op0->type();
    const Type* op1_type = op1->type();
    const Type* op0_rtype;
    const Type* op1_rtype;
    const Type* type = mTypeMgr.calc_type2(opcode, op0_type, op1_type, op0_rtype, op1_rtype);
    if ( type == NULL ) {
      // type mismatch
      cerr << "type mismatch" << endl;
      return NULL;
    }

    if ( op0_rtype != op0_type ) {
      // キャストノードの挿入
    }

    if ( op1_rtype != op1_type ) {
      // キャストノードの挿入
    }
    node = new_BinOp(opcode, type, op0, op1);
    return node;
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
    node = new_TriOp(opcode, type, op0, op1, op2);
    return node;
  }

  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief プライマリ式の解析を行う．
// @param[in] ast_expr 式を表す構文木
// @param[in] scope 現在のスコープ
IrHandle*
IrMgr::elab_primary(const AstExpr* ast_expr,
		    Scope* scope)
{
  switch ( ast_expr->expr_type() ) {
  case kSymbolExpr:
    {
      const AstSymbol* symbol = ast_expr->symbol();
      IrHandle* h = scope->find(symbol->str_val());
      if ( h == NULL ) {
	// symbol not found
	return NULL;
      }
      return h;
    }

  case kArrayRef:
    {
      // 配列本体やインデックス値自体は右辺値
      IrHandle* h = elab_primary(ast_expr->body(), scope);
      if ( h == NULL ) {
	return NULL;
      }
      IrNode* base = new_Load(h);
      if ( base->type()->type_id() != kArrayType ) {
	// base is not an array
	return NULL;
      }

      IrNode* offset = elab_rhs(ast_expr->index(), scope);
      if ( offset->type()->type_id() != kIntType ) {
	// offset is not an integer
	return NULL;
      }

      return new_ArrayRef(base, offset);
    }

  case kMemberRef:
    {
      const AstExpr* body = ast_expr->body();
      const AstSymbol* member_symbol = ast_expr->member();
      IrHandle* h = elab_primary(body, scope);
      switch ( h->handle_type() ) {
      case IrHandle::kScope:
	{
	  Scope* scope1 = h->scope();
	  IrHandle* h1 = scope1->find(member_symbol->str_val());
	  if ( h1 == NULL ) {
	    // member_symbol not found
	    return NULL;
	  }
	  return h1;
	}

      case IrHandle::kVar:
	{
	  const Var* var = h->var();
	  const Type* type = var->value_type();
	  ShString member_name = member_symbol->str_val();
	  // type のメンバに member_name があることを確認する．
	  IrNode* base = new_Load(h);
	  const Var* member;
	  return new_MemberRef(base, member);
	}

      default:
	ASSERT_NOT_REACHED;
	break;
      }
    }
    break;

  default:
    break;
  }

  ASSERT_NOT_REACHED;
  return NULL;
}

// @brief 式から関数の解決を行う．
// @param[in] expr 式
// @param[in] scope 現在のスコープ
// @param[in] node 関数呼び出しノード
bool
IrMgr::resolve_func(const AstExpr* expr,
		    Scope* scope,
		    IrNode* node)
{
  IrHandle* h = elab_primary(expr, scope);
  if ( h == NULL ) {
    // expr not found
    return false;
  }
  if ( h->handle_type() != IrHandle::kFunction ) {
    // expr is not a function;
    return false;
  }
  const Function* func = h->function();
  // func の型と node の arglist の型をチェック

  // node に func をセット
  return true;
}

// @brief スコープを生成する．
// @param[in] parent_scope 親のスコープ
// @param[in] name スコープ名
Scope*
IrMgr::new_scope(Scope* parent_scope,
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
IrMgr::new_var(ShString name,
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
IrMgr::new_function(ShString name,
		    const Type* type)
{
  ymuint index = mFuncList.size();
  Function* func = new Function(name, type, index);
  mFuncList.push_back(func);

  return func;
}

END_NAMESPACE_YM_YMSL
