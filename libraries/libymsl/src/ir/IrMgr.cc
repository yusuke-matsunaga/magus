
/// @file IrMgr.cc
/// @brief IrMgr の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"

#include "YmslCompiler.h"

#include "AstMgr.h"
#include "AstStatement.h"
#include "AstSymbol.h"

#include "Scope.h"
#include "IrCodeBlock.h"
#include "IrToplevel.h"
#include "IrNode.h"
#include "IrHandle.h"

#include "VsmModule.h"
#include "VsmFunction.h"


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
    Scope* scope = *p;
    // デストラクタの明示的呼び出し．
    scope->~Scope();
  }
  mScopeList.clear();

  mUndefList.clear();

  mTypeMgr.clear();

  mAlloc.destroy();
}

// @brief 抽象構文木から中間表現を生成する．
// @param[in] ast_root 抽象構文木の根のノード
// @param[in] toplevel_scope トップレベルのスコープ
// @param[in] code_block 生成された中間表現を格納するオブジェクト
//
// エラーが起きたら false を返す．
bool
IrMgr::elaborate(const AstStatement* ast_root,
		 ShString name,
		 IrToplevel& toplevel_block)
{
  ASSERT_COND( ast_root->stmt_type() == AstStatement::kToplevel );

  mFuncCallList.clear();
  mUndefList.clear();

  // 中間表現を作る．
  Scope* toplevel_scope = new_scope(NULL, name);

  ymuint head_num = ast_root->headlist_num();
  for (ymuint i = 0; i < head_num; ++ i) {
    const AstStatement* stmt = ast_root->headlist_elem(i);
    switch ( stmt->stmt_type() ) {
    case AstStatement::kImport:
      {
	const AstSymbol* module_symbol = stmt->import_module();
	ShString module_name = module_symbol->str_val();
	const AstSymbol* alias_symbol = stmt->import_alias();
	pair<VsmModule*, Scope*> p = import_module(module_name);
	VsmModule* module = p.first;
	Scope* scope = p.second;
	toplevel_block.add_imported_module(module);
	ShString alias_name = module_name;
	if ( alias_symbol != NULL ) {
	  alias_name = alias_symbol->str_val();
	}
	IrHandle* scope_handle = new_ScopeHandle(alias_name, scope);
	toplevel_scope->add(scope_handle);
      }
      break;

    default:
      ASSERT_NOT_REACHED;
      break;
    }
  }

  ymuint stmt_num = ast_root->stmtlist_num();
  for (ymuint i = 0; i < stmt_num; ++ i) {
    const AstStatement* stmt = ast_root->stmtlist_elem(i);
    elab_stmt(stmt, toplevel_scope, NULL, NULL, toplevel_block);
  }

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

// @brief モジュールを import する．
// @param[in] name モジュール名
pair<VsmModule*, Scope*>
IrMgr::import_module(ShString name)
{
  VsmModule* module;
  Scope* scope;
  if ( !mModuleDict.find(name, module) ) {
    module = NULL;

    // import したモジュールに対応するスコープを作る．
    scope = module2scope(module, name);

    mModuleDict.add(name, module);
    mScopeDict.add(module, scope);
  }
  else {
    bool stat = mScopeDict.find(module, scope);
    ASSERT_COND( stat );
  }
  return make_pair(module, scope);
}

// @brief モジュールに対応するスコープを作る．
// @param[in] module モジュール
// @param[in] parent_scope 親のスコープ
// @param[in] name 名前
Scope*
IrMgr::module2scope(VsmModule* module,
		    ShString name)
{
  Scope* scope = new_scope(NULL, name);
  for (ymuint i = 0; i < module->imported_module_num(); ++ i) {
    VsmModule* sub_module = module->imported_module(i);
    // Scope* sub_scope を mModuleScopeDict から取ってくる．
    Scope* sub_scope;
    bool stat = mScopeDict.find(sub_module, sub_scope);
    ASSERT_COND( stat );

    IrHandle* h = new_ScopeHandle(name, sub_scope);
    scope->add(h);
  }
  for (ymuint i = 0; i < module->exported_function_num(); ++ i) {
    VsmFunction* func = module->exported_function(i);

    IrHandle* h = new_FuncHandle(func->name(), func->type());
    scope->add(h);
  }
  for (ymuint i = 0; i < module->exported_variable_num(); ++ i) {
    VsmVar* var = module->exported_variable(i);
    //scope.add();
  }
  return scope;
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

  // func の型と node の arglist の型をチェック
  // 場合によってはキャストノードを挿入する．

  // node に func をセット
  node->set_func_addr(h);
  return true;
}

// @brief スコープを生成する．
// @param[in] parent_scope 親のスコープ
// @param[in] name スコープ名
Scope*
IrMgr::new_scope(Scope* parent_scope,
		 ShString name)
{
  Scope* global_scope = NULL;
  if ( parent_scope != NULL ) {
    // 基本的には親のグローバルスコープを受け継ぐ
    global_scope = parent_scope->global_scope();
    if ( global_scope == NULL ) {
      // グローバルスコープを持たないスコープは
      // グローバルスコープだけ．
      global_scope = parent_scope;
      parent_scope = NULL;
    }
  }
  void* p = mAlloc.get_memory(sizeof(Scope));
  Scope* scope = new (p) Scope(parent_scope, global_scope, name);
  mScopeList.push_back(scope);

  return scope;
}

END_NAMESPACE_YM_YMSL
