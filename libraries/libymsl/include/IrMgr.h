#ifndef IRMGR_H
#define IRMGR_H

/// @file IrMgr.h
/// @brief IrMgr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "TypeMgr.h"
#include "YmUtils/ShString.h"
#include "YmUtils/SimpleAlloc.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrMgr IrMgr.h "IrMgr.h"
/// @brief IR オブジェクトを管理するクラス
//////////////////////////////////////////////////////////////////////
class IrMgr
{
public:

  /// @brief コンストラクタ
  IrMgr();

  /// @brief デストラクタ
  ~IrMgr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief クリアする．
  void
  clear();

  /// @brief 抽象構文木から中間表現を生成する．
  /// @param[in] ast_root 抽象構文木の根のノード
  /// @param[in] node_list 生成したノードのリスト
  ///
  /// エラーが起きたら false を返す．
  bool
  elaborate(const AstStatement* ast_root,
	    vector<IrNode*>& node_list);


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
  IrNode*
  elab_rhs(const AstExpr* ast_expr,
	   Scope* scope);

  /// @brief 右辺式の実体化を行う．(プライマリ用)
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  IrNode*
  elab_rhs_primary(const AstExpr* ast_expr,
		   Scope* scope);

  /// @brief プライマリ式の解析を行う．
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  IrHandle*
  analyze_primary(const AstExpr* ast_expr,
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
  // IrNode の派生クラスを生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief true 定数を生成する．
  IrNode*
  new_True();

  /// @brief False 定数を生成する．
  IrNode*
  new_False();

  /// @brief 整数値定数を生成する．
  /// @param[in] val 値
  IrNode*
  new_IntConst(int val);

  /// @brief 実数値定数を生成する．
  /// @param[in] val 値
  IrNode*
  new_FloatConst(double val);

  /// @brief 文字列定数を生成する．
  /// @param[in] val 値
  IrNode*
  new_StringConst(const char* val);

  /// @brief 単項演算式を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] type 出力の型
  /// @param[in] opr1 オペランド
  IrNode*
  new_UniOp(OpCode opcode,
	    const Type* type,
	    IrNode* opr1);

  /// @brief 二項演算式を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] type 出力の型
  /// @param[in] opr1, opr2 オペランド
  IrNode*
  new_BinOp(OpCode opcode,
	    const Type* type,
	    IrNode* opr1,
	    IrNode* opr2);

  /// @brief 三項演算式を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] type 出力の型
  /// @param[in] opr1, opr2, opr3 オペランド
  IrNode*
  new_TriOp(OpCode opcode,
	    const Type* type,
	    IrNode* opr1,
	    IrNode* opr2,
	    IrNode* opr3);

  /// @brief load 文を生成する．
  /// @param[in] addr アドレス
  IrNode*
  new_Load(IrHandle* addr);

  /// @brief store 文を生成する．
  /// @param[in] addr アドレス
  /// @param[in] val 書き込む値
  IrNode*
  new_Store(IrHandle* addr,
	    IrNode* val);

  /// @brief 自己代入型の単項演算を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] lhs_addr 左辺式
  IrNode*
  new_InplaceUniOp(OpCode opcode,
		   IrHandle* lhs_addr);

  /// @brief 自己代入型の二項演算を生成する．
  /// @param[in] opcode オペコード
  /// @param[in] lhs_addr 左辺式
  /// @param[in] opr オペランド
  IrNode*
  new_InplaceBinOp(OpCode opcode,
		   IrHandle* lhs_addr,
		   IrNode* opr);

  /// @brief 関数呼び出し式を生成する．
  /// @param[in] func_addr 関数アドレス
  /// @param[in] arglist 引数のリスト
  IrNode*
  new_FuncCall(IrHandle* func_addr,
	       const vector<IrNode*>& arglist);

  /// @brief リターン命令を生成する．
  /// @param[in] ret_val 返り値
  IrNode*
  new_Return(IrNode* ret_val = NULL);

  /// @brief ジャンプ系のノードを生成する．
  /// @param[in] opcode オペコード
  /// @param[in] label ジャンプ先のラベル
  /// @param[in] cond 条件
  IrNode*
  new_Jump(OpCode opcode,
	   IrNode* label,
	   IrNode* cond = NULL);

  /// @brief ラベルノードを生成する．
  IrNode*
  new_Label();


private:
  //////////////////////////////////////////////////////////////////////
  // IrHandle の派生クラスを生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープ参照を生成する．
  /// @param[in] scope スコープ
  IrHandle*
  new_ScopeHandle(Scope* scope);

  /// @brief 変数参照を生成する．
  /// @param[in] var 変数
  IrHandle*
  new_VarHandle(const Var* var);

  /// @brief 関数参照を生成する．
  /// @param[in] func 関数
  IrHandle*
  new_FuncHandle(const Function* func);

  /// @brief 定数参照を生成する．
  /// @param[in] name 名前
  /// @param[in] node 定数ノード
  IrHandle*
  new_ConstHandle(ShString name,
		  IrNode* node);

  /// @brief ラベル参照を生成する．
  /// @param[in] name 名前
  /// @param[in] label ラベル
  IrHandle*
  new_LabelHandle(ShString name,
		  IrNode* label);

  /// @brief 名前付き型参照を生成する．
  /// @param[in] type 型
  IrHandle*
  new_TypeHandle(const Type* type,
		 Scope* scope);

  /// @brief 配列参照を生成する．
  /// @param[in] array 配列
  /// @param[in] index インデックス
  IrHandle*
  new_ArrayRef(IrNode* array,
	       IrNode* index);

  /// @brief クラスメンバ参照を生成する．
  /// @param[in] obj オブジェクト
  /// @param[in] var メンバ変数
  IrHandle*
  new_MemberRef(IrNode* obj,
		const Var* var);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // メモリアロケータ
  SimpleAlloc mAlloc;

  // 型を管理するオブジェクト
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

#endif // IRMGR_H
