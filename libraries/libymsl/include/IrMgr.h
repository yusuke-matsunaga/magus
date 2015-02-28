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
#include "YmUtils/HashMap.h"
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
  /// @param[in] name モジュール名
  /// @param[in] code_block 生成された中間表現を格納するオブジェクト
  ///
  /// エラーが起きたら false を返す．
  bool
  elaborate(const AstStatement* ast_root,
	    ShString name,
	    IrToplevel& code_block);


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief モジュールを import する．
  /// @param[in] name モジュール名
  pair<VsmModule*, Scope*>
  import_module(ShString name);

  /// @brief モジュールに対応するスコープを作る．
  /// @param[in] module モジュール
  /// @param[in] name 名前
  Scope*
  module2scope(VsmModule* module,
	       ShString name);

  /// @brief 式から関数の解決を行う．
  /// @param[in] expr 式
  /// @param[in] scope 現在のスコープ
  /// @param[in] node 関数呼び出しノード
  bool
  resolve_func(const AstExpr* expr,
	       Scope* scope,
	       IrNode* node);

  /// @brief 要素の生成を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  /// @param[in] start_label ブロックの開始ラベル
  /// @param[in] end_label ブロックの終了ラベル
  /// @param[in] code_block 生成された中間表現を格納するオブジェクト
  void
  elab_stmt(const AstStatement* stmt,
	    Scope* scope,
	    IrNode* start_label,
	    IrNode* end_label,
	    IrCodeBlock& code_block);

  /// @brief 名前がローカルに重複していないかチェックする．
  /// @param[in] ast_name 名前を表す AST
  /// @param[in] scope 現在のスコープ
  /// @retval true 重複していない．
  /// @retval false 重複している．
  bool
  check_name(const AstSymbol* name,
	     Scope* scope);

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
  /// @param[in] code_block 生成された中間表現を格納するオブジェクト
  ///
  /// stmt は kFuncDecl でなければならない．
  void
  reg_func(const AstStatement* stmt,
	   Scope* scope,
	   IrCodeBlock& code_block);

  /// @brief 定数の定義を行う．
  /// @param[in] name_symbol 名前
  /// @param[in] ast_type 型
  /// @param[in] ast_expr 式
  /// @param[in] scope 現在のスコープ
  void
  reg_const(const AstSymbol* name_symbol,
	    const AstType* ast_type,
	    const AstExpr* ast_expr,
	    Scope* scope);

  /// @brief 型の参照を解決する．
  /// @param[in] asttype 型を表す構文木
  /// @param[in] scope 現在のスコープ
  ///
  /// 解決できない時には NULL を返す．
  const Type*
  resolve_type(const AstType* asttype,
	       Scope* scope);

  /// @brief 式の実体化を行う．
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  IrNode*
  elab_expr(const AstExpr* ast_expr,
	    Scope* scope);

  /// @brief プライマリ式の解析を行う．
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  IrHandle*
  elab_primary(const AstExpr* ast_expr,
	       Scope* scope);


private:
  //////////////////////////////////////////////////////////////////////
  // IrNode の派生クラスを生成する関数
  //////////////////////////////////////////////////////////////////////

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

  /// @brief ジャンプノードを生成する．
  /// @param[in] label ジャンプ先のラベル
  IrNode*
  new_Jump(IrNode* label);

  /// @brief 分岐ノードを生成する．
  /// @param[in] label ジャンプ先のラベル
  /// @param[in] cond 条件
  IrNode*
  new_BranchTrue(IrNode* label,
		 IrNode* cond);

  /// @brief 分岐ノードを生成する．
  /// @param[in] label ジャンプ先のラベル
  /// @param[in] cond 条件
  IrNode*
  new_BranchFalse(IrNode* label,
		  IrNode* cond);

  /// @brief ラベルノードを生成する．
  IrNode*
  new_Label();


private:
  //////////////////////////////////////////////////////////////////////
  // IrHandle の派生クラスを生成する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief スコープ参照を生成する．
  /// @param[in] name 名前
  /// @param[in] scope スコープ
  IrHandle*
  new_ScopeHandle(ShString name,
		  Scope* scope);

  /// @brief 変数参照を生成する．
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  /// @param[in] global グローバル変数の時 true とするフラグ
  IrHandle*
  new_VarHandle(ShString name,
		const Type* value_type,
		bool global);

  /// @brief 関数参照を生成する．
  /// @param[in] name 変数名
  /// @param[in] value_type 型
  IrHandle*
  new_FuncHandle(ShString name,
		 const Type* value_type);

  /// @brief ブール定数を生成する．
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrHandle*
  new_BooleanConst(ShString name,
		   Ymsl_BOOLEAN const_val);

  /// @brief 整数定数を生成する．
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrHandle*
  new_IntConst(ShString name,
	       Ymsl_INT const_val);

  /// @brief 実数定数を生成する．
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrHandle*
  new_FloatConst(ShString name,
		 Ymsl_FLOAT const_val);

  /// @brief 文字列定数を生成する．
  /// @param[in] name 名前
  /// @param[in] const_val 定数値
  IrHandle*
  new_StringConst(ShString name,
		  Ymsl_STRING const_val);

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
		IrHandle* var);

  /// @brief スコープを生成する．
  /// @param[in] parent_scope 親のスコープ
  /// @param[in] name スコープ名
  Scope*
  new_scope(Scope* parent_scope,
	    ShString name = ShString());


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

  // モジュールを保持するハッシュ表
  HashMap<ShString, VsmModule*> mModuleDict;

  // モジュールに対応したスコープを保持するハッシュ表
  HashMap<VsmModule*, Scope*> mScopeDict;

};

END_NAMESPACE_YM_YMSL

BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// HashFunc<VsmModule*> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<nsYmsl::VsmModule*>
{
  ymuint
  operator()(nsYmsl::VsmModule* key) const
  {
    return reinterpret_cast<ympuint>(key) / sizeof(void*);
  }
};

END_NAMESPACE_YM

#endif // IRMGR_H
