
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"
#include "TypeMgr.h"

#include "AstStatement.h"
#include "AstSymbol.h"
#include "AstType.h"

#include "Function.h"
#include "Scope.h"
#include "Type.h"
#include "IrCodeBlock.h"
#include "IrHandle.h"
#include "IrFunction.h"
#include "IrNode.h"
#include "IrToplevel.h"
#include "IrInterp.h"

#include "IrPrinter.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief 要素の生成を行う．
// @param[in] stmt 文
// @param[in] scope 現在のスコープ
// @param[in] start_label ブロックの開始ラベル
// @param[in] end_label ブロックの終了ラベル
// @param[in] code_block 生成された中間表現を格納するオブジェクト
void
IrMgr::elab_stmt(const AstStatement* stmt,
		 Scope* scope,
		 IrNode* start_label,
		 IrNode* end_label,
		 IrCodeBlock& code_block)
{
  switch ( stmt->stmt_type() ) {
  case AstStatement::kBlock:
    {
      Scope* block_scope = new_scope(scope);
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	elab_stmt(stmt1, block_scope, start_label, end_label, code_block);
      }
    }
    break;

  case AstStatement::kBreak:
    {
      if ( end_label == NULL ) {
	// not inside loop
	return;
      }
      IrNode* node = new_Jump(end_label);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kConstDecl:
    reg_const(stmt, scope);
    break;

  case AstStatement::kContinue:
    {
      if ( start_label == NULL ) {
	// not inside loop
	return;
      }
      IrNode* node = new_Jump(start_label);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kDecr:
    {
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }
      // lhs_handle の指しているものが int 型の変数かチェック

      IrNode* node = new_InplaceUniOp(kOpDec, lhs_handle);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kDoWhile:
    {
      IrNode* start1 = new_Label();
      IrNode* end1 = new_Label();
      code_block.add_node(start1);
      elab_stmt(stmt->stmt(), scope, start1, end1, code_block);
      IrNode* cond = elab_expr(stmt->expr(), scope);
      IrNode* node1 = new_BranchTrue(start1, cond);
      code_block.add_node(node1);
      code_block.add_node(end1);
    }
    break;

  case AstStatement::kEnumDecl:
    reg_enum(stmt, scope);
    break;

  case AstStatement::kAssignment:
    {
      IrNode* rhs = elab_expr(stmt->expr(), scope);
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
      code_block.add_node(node);
    }
    break;

  case AstStatement::kInplaceOp:
    {
      IrNode* rhs = elab_expr(stmt->expr(), scope);
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
      OpCode opcode = stmt->opcode();
      IrNode* node = new_InplaceBinOp(opcode, lhs_handle, rhs);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kExpr:
    {
      IrNode* node = elab_expr(stmt->expr(), scope);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kFor:
    {
      Scope* for_scope = new_scope(scope);
      elab_stmt(stmt->init_stmt(), for_scope, NULL, NULL, code_block);
      IrNode* start1 = new_Label();
      code_block.add_node(start1);
      IrNode* end1 = new_Label();
      IrNode* cond = elab_expr(stmt->expr(), for_scope);
      IrNode* node1 = new_BranchFalse(end1, cond);
      code_block.add_node(node1);
      elab_stmt(stmt->stmt(), for_scope, start1, end1, code_block);
      elab_stmt(stmt->next_stmt(), for_scope, start1, end1, code_block);
      IrNode* node2 = new_Jump(start1);
      code_block.add_node(node2);
      code_block.add_node(end1);
    }
    break;

  case AstStatement::kFuncDecl:
    reg_func(stmt, scope, code_block);
    break;

  case AstStatement::kGoto:
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
      IrNode* node = new_Jump(label_node);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kIf:
    {
      IrNode* cond = elab_expr(stmt->expr(), scope);
      IrNode* label1 = new_Label();
      IrNode* label2 = new_Label();
      IrNode* node1 = new_BranchFalse(label1, cond);
      code_block.add_node(node1);
      elab_stmt(stmt->stmt(), scope, start_label, end_label, code_block);
      IrNode* node2 = new_Jump(label2);
      code_block.add_node(node2);
      code_block.add_node(label1);
      elab_stmt(stmt->else_stmt(), scope, start_label, end_label, code_block);
      code_block.add_node(label2);
    }
    break;

  case AstStatement::kImport:
    break;

  case AstStatement::kIncr:
    {
      IrHandle* lhs_handle = elab_primary(stmt->lhs_expr(), scope);
      if ( lhs_handle == NULL ) {
	// エラー
	return;
      }
      IrNode* node = new_InplaceUniOp(kOpInc, lhs_handle);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kLabel:
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
      code_block.add_node(label_node);
    }
    break;

  case AstStatement::kNullStmt:
    break;

  case AstStatement::kReturn:
    {
      const AstExpr* expr = stmt->expr();
      IrNode* ret_val = NULL;
      if ( expr != NULL ) {
	ret_val = elab_expr(expr, scope);
      }
      IrNode* node = new_Return(ret_val);
      code_block.add_node(node);
    }
    break;

  case AstStatement::kSwitch:
    {
      IrNode* cond = elab_expr(stmt->expr(), scope);
      ymuint n = stmt->switch_num();
      for (ymuint i = 0; i < n; ++ i) {
	vector<IrNode*> node_list1;
	elab_stmt(stmt->case_stmt(i), scope, start_label, end_label, code_block);
      }
    }
    break;

  case AstStatement::kToplevel:
    {
      ymuint n = stmt->stmtlist_num();
      for (ymuint i = 0; i < n; ++ i) {
	const AstStatement* stmt1 = stmt->stmtlist_elem(i);
	elab_stmt(stmt1, scope, NULL, NULL, code_block);
      }
    }
    break;

  case AstStatement::kVarDecl:
    {
      const AstSymbol* ast_name = stmt->name();
      if ( !check_name(ast_name, scope) ) {
	// 名前が重複している．
	return;
      }

      const AstType* ast_type = stmt->type();
      const Type* type = resolve_type(ast_type, scope);
      if ( type == NULL ) {
	// エラーメッセージをどこで出すか考える．
	return;
      }

      ShString name = ast_name->str_val();
      bool global = (scope->global_scope() == NULL);
      Var* var = new_var(name, type, global);

      code_block.add_var(var);

      IrHandle* h = new_VarHandle(var);
      scope->add(h);

      const AstExpr* ast_expr = stmt->expr();
      if ( ast_expr != NULL ) {
	IrNode* expr = elab_expr(ast_expr, scope);
	if ( expr == NULL ) {
	  // エラー
	  return;
	}
	IrNode* node = new_Store(h, expr);
	code_block.add_node(node);
      }
    }
    break;

  case AstStatement::kWhile:
    {
      IrNode* start1 = new_Label();
      code_block.add_node(start1);
      IrNode* cond = elab_expr(stmt->expr(), scope);
      IrNode* end1 = new_Label();
      IrNode* node1 = new_BranchFalse(end1, cond);
      code_block.add_node(node1);
      elab_stmt(stmt->stmt(), scope, start1, end1, code_block);
      IrNode* node2 = new_Jump(start1);
      code_block.add_node(node2);
      code_block.add_node(end1);
    }
    break;

  }
}

// @brief 名前がローカルに重複していないかチェックする．
// @param[in] ast_name 名前を表す AST
// @param[in] scope 現在のスコープ
// @retval true 重複していない．
// @retval false 重複している．
bool
IrMgr::check_name(const AstSymbol* ast_name,
		  Scope* scope)
{
  ShString name = ast_name->str_val();
  IrHandle* h = scope->find_local(name);
  if ( h != NULL ) {
    // name が重複している．
    // previous definition is h->file_region()
    return false;
  }

  return true;
}

// @brief enum 型の定義を行う．
// @param[in] stmt 文
//
// stmt は kEnumDecl でなければならない．
void
IrMgr::reg_enum(const AstStatement* stmt,
		Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == AstStatement::kEnumDecl );

  const AstSymbol* name_symbol = stmt->name();
  if ( !check_name(name_symbol, scope) ) {
    // 名前が重複している．
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
      IrNode* node = elab_expr(ec_expr, scope);
      if ( node == NULL ) {
	// エラー
	return;
      }
      if ( !node->is_static() ) {
	// ec_expr が定数式ではない．
	return;
      }
      if ( node->value_type() != mTypeMgr.int_type() ) {
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

  ShString name = name_symbol->str_val();
  const Type* type = mTypeMgr.enum_type(name, elem_list);
  Scope* enum_scope = new_scope(scope, name);
  for (ymuint i = 0; i < n; ++ i) {
    ShString name = type->enum_elem_name(i);
    int val = type->enum_elem_val(i);
    const ConstVal* const_val = new_IntConst(val);
    IrHandle* h = new_ConstHandle(name, const_val);
    enum_scope->add(h);
  }

  IrHandle* h = new_TypeHandle(type, enum_scope);
  scope->add(h);
}

// @brief 関数の定義を行う．
// @param[in] stmt 文
// @param[in] scope 現在のスコープ
// @param[in] code_block 生成された中間表現を格納するオブジェクト
//
// stmt は kFuncDecl でなければならない．
void
IrMgr::reg_func(const AstStatement* stmt,
		Scope* scope,
		IrCodeBlock& code_block)

{
  ASSERT_COND( stmt->stmt_type() == AstStatement::kFuncDecl );

  const AstSymbol* name_symbol = stmt->name();
  if ( !check_name(name_symbol, scope) ) {
    // 名前が重複している．
    return;
  }

  // 関数のスコープを生成する．
  Scope* global_scope = scope->global_scope();
  if ( global_scope == NULL ) {
    global_scope = scope;
  }
  Scope* func_scope = new_scope(global_scope);

  // 出力の型を解決する．
  const AstType* ast_type = stmt->type();
  const Type* output_type = resolve_type(ast_type, scope);
  if ( output_type == NULL ) {
    // エラーメッセージをどこで出すか考える．
    return;
  }

  // 入力に関するいくつかの処理を行う．
  ymuint np = stmt->param_num();
  vector<ShString> input_name_list(np);
  vector<const Type*> input_type_list(np);
  for (ymuint i = 0; i < np; ++ i) {
    // 名前の重複チェック
    const AstSymbol* ast_name = stmt->param_name(i);
    if ( !check_name(ast_name, func_scope) ) {
      // 名前が重複していた．
      return;
    }
    input_name_list[i] = ast_name->str_val();

    // 型の解決
    const AstType* ast_type = stmt->param_type(i);
    const Type* type = resolve_type(ast_type, scope);
    if ( type == NULL ) {
      // エラーメッセージをどこで出すか考える．
      return;
    }
    input_type_list[i] = type;
  }

  // 関数の生成
  ShString name = name_symbol->str_val();
  const Type* ftype = mTypeMgr.function_type(output_type, input_type_list);
  Function* func = new_function(name, ftype);

  IrHandle* h = new_FuncHandle(func);
  scope->add(h);

  // 引数の生成
  vector<const Var*> arg_list(np);
  vector<IrNode*> arg_init_list(np);
  for (ymuint i = 0; i < np; ++ i) {
    Var* var = new_var(input_name_list[i], input_type_list[i], false);
    arg_list[i] =  var;
    IrHandle* h = new_VarHandle(var);
    func_scope->add(h);

    // デフォルト値の式の生成
    // これは関数外のスコープで行う．
    const AstExpr* ast_expr = stmt->param_expr(i);
    IrNode* node = NULL;
    if ( ast_expr != NULL ) {
      node = elab_expr(ast_expr, scope);
      if ( node == NULL ) {
	return;
      }
    }
    arg_init_list[i] = node;
  }

  IrToplevel& toplevel = code_block.toplevel();
  // 関数の内部表現の生成
  IrFunction* ir_func = new IrFunction(toplevel, func, arg_list, arg_init_list);
  toplevel.add_function(ir_func);

  // 関数内部のノードの生成
  elab_stmt(stmt->stmt(), func_scope, NULL, NULL, *ir_func);
}

// @brief 定数の定義を行う．
// @param[in] stmt 文
//
// stmt は kConstDecl でなければならない．
void
IrMgr::reg_const(const AstStatement* stmt,
		 Scope* scope)
{
  ASSERT_COND( stmt->stmt_type() == AstStatement::kConstDecl );

  cout << "reg_const" << endl;

  const AstSymbol* name_symbol = stmt->name();
  if ( !check_name(name_symbol, scope) ) {
    // 名前が重複していた．
    cout << name_symbol->str_val() << ": already defined" << endl;
    return;
  }

  const AstType* ast_type = stmt->type();
  const Type* type = resolve_type(ast_type, scope);
  if ( type == NULL ) {
    // エラーメッセージをどこで出すか考える．
    cout << "type resolve error" << endl;
    return;
  }

  const AstExpr* ast_expr = stmt->expr();
  IrNode* node = elab_expr(ast_expr, scope);
  if ( node == NULL ) {
    cout << "eval_expr() == NULL" << endl;
    return;
  }

  // node->type() が type と互換性があるかをチェック

  // node が定数式かチェック
  if ( !node->is_static() ) {
    // ast_expr is not a constant
    cout << "not a constant" << endl;
    return;
  }

  {
    IrPrinter printer(cout);
    printer.print_node(node);
  }
  IrInterp interp;
  const ConstVal* const_val = NULL;
  switch ( node->value_type()->type_id() ) {
  case kBooleanType:
    {
      bool val = interp.eval_boolean(node);
      if ( val ) {
	const_val = new_True();
      }
      else {
	const_val = new_False();
      }
    }
    break;

  case kIntType:
    {
      Ymsl_INT val = interp.eval_int(node);
      const_val = new_IntConst(val);
    }
    break;

  case kFloatType:
    {
      Ymsl_FLOAT val = interp.eval_float(node);
      const_val = new_FloatConst(val);
    }
    break;

  default:
    ASSERT_NOT_REACHED;
  }
  ASSERT_COND( const_val != NULL );

  ShString name = name_symbol->str_val();
  IrHandle* h = new_ConstHandle(name, const_val);
  scope->add(h);
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
  node->set_func_addr(h);
  return true;
}

END_NAMESPACE_YM_YMSL
