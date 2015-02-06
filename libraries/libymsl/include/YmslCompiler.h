#ifndef YMSLCOMPILER_H
#define YMSLCOMPILER_H

/// @file YmslCompiler.h
/// @brief YmslCompiler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "IrMgr.h"
#include "TypeMgr.h"
#include "YmUtils/IDO.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslCompiler YmslCompiler.h "YmslCompiler.h"
/// @brief YMSL 用のコンパイラ
//////////////////////////////////////////////////////////////////////
class YmslCompiler
{
public:

  /// @brief コンストラクタ
  YmslCompiler();

  /// @brief デストラクタ
  ~YmslCompiler();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief コンパイルする．
  /// @param[in] ido 入力データ
  /// @return 成功したら true を返す．
  bool
  compile(IDO& ido);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 要素の生成を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  /// @param[in] start_label ブロックの開始ラベル
  /// @param[in] end_label ブロックの終了ラベル
  /// @param[out] node_list 生成されたノードのリスト
  void
  elab_stmt(const AstStatement* stmt,
	    Scope* scope,
	    IrNode* start_label,
	    IrNode* end_label,
	    vector<IrNode*>& node_list);

  /// @brief enum 型の定義を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  ///
  /// stmt は kEnumDecl でなければならない．
  void
  reg_enum(const AstStatement* stmt,
	   Scope* scope);

  /// @brief 関数の定義を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  ///
  /// stmt は kFuncDecl でなければならない．
  void
  reg_func(const AstStatement* stmt,
	   Scope* scope);

  /// @brief 変数の定義を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  ///
  /// stmt は kVarDecl でなければならない．
  void
  reg_var(const AstStatement* stmt,
	  Scope* scope);

  /// @brief 定数の定義を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  ///
  /// stmt は kConstDecl でなければならない．
  void
  reg_const(const AstStatement* stmt,
	    Scope* scope);

  /// @brief 型の参照を解決する．
  /// @param[in] asttype 型を表す構文木
  /// @param[in] scope 現在のスコープ
  ///
  /// 解決できない時には NULL を返す．
  const Type*
  resolve_type(const AstType* asttype,
	       Scope* scope);

  /// @brief 右辺式の実体化を行う．
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  /// @param[in] node_list ノードを収めるリスト
  IrNode*
  elab_rhs(const AstExpr* ast_expr,
	   Scope* scope,
	   vector<IrNode*>& node_list);

  /// @brief 右辺式の実体化を行う．(プライマリ用)
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  /// @param[in] node_list ノードを収めるリスト
  IrNode*
  elab_rhs_primary(const AstExpr* ast_expr,
		   Scope* scope,
		   vector<IrNode*>& node_list);

  /// @brief 左辺式の実体化を行う．
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  /// @param[out] var 変数
  /// @param[out] base ベース値
  /// @param[out] offset オフセット
  /// @param[in] node_list ノードを収めるリスト
  ///
  /// エラーが起きたら false を返す．
  bool
  elab_lhs(const AstExpr* ast_expr,
	   Scope* scope,
	   const Var*& var,
	   IrNode*& base,
	   IrNode*& offset,
	   vector<IrNode*>& node_list);

  /// @brief 式からスコープの解決を行う．
  /// @param[in] expr 式
  /// @param[in] scopde 現在のスコープ
  Scope*
  resolve_scope(const AstExpr* expr,
		Scope* scope);

  /// @brief 式からシンボルの解決を行う．
  /// @param[in] expr 式
  /// @param[in] scopde 現在のスコープ
  SymHandle*
  resolve_symbol(const AstExpr* expr,
		 Scope* scope);

  /// @brief 式から関数の解決を行う．
  /// @param[in] expr 式
  /// @param[in] scope 現在のスコープ
  /// @param[in] node 関数呼び出しノード
  bool
  resolve_func(const AstExpr* expr,
	       Scope* scope,
	       IrNode* node);

  /// @brief スコープを生成する．
  /// @param[in] parent_scope 親のスコープ
  /// @param[in] name スコープ名
  Scope*
  new_scope(Scope* parent_scope,
	    ShString name = ShString());

  /// @brief 変数を生成する．
  /// @param[in] name 名前
  /// @param[in] type 型
  Var*
  new_var(ShString name,
	  const Type* type);

  /// @brief 関数を生成する．
  /// @param[in] name 名前
  /// @param[in] type 関数の型
  Function*
  new_function(ShString name,
	       const Type* type);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // IR を管理するマネージャ
  IrMgr mIrMgr;

  // 型を管理するマネージャ
  TypeMgr mTypeMgr;

  // mFuncCallList の要素の構造体
  struct FuncCallStub {

    // コンストラクタ
    FuncCallStub(const AstExpr* expr = NULL,
		 Scope* scope = NULL,
		 IrNode* node = NULL) :
      mExpr(expr),
      mScope(scope),
      mNode(node) { }

    // 式
    const AstExpr* mExpr;

    // スコープ
    Scope* mScope;

    // ノード
    IrNode* mNode;

  };

  // 関数呼び出しのリスト
  vector<FuncCallStub> mFuncCallList;

  // 未定義ラベルのリスト
  vector<IrNode*> mUndefList;

  // スコープのリスト
  vector<Scope*> mScopeList;

  // 変数のリスト
  vector<Var*> mVarList;

  // 関数のリスト
  vector<Function*> mFuncList;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLCOMPILER_H
