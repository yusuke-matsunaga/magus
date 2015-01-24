#ifndef YMSLCOMPILER_H
#define YMSLCOMPILER_H

/// @file YmslCompiler.h
/// @brief YmslCompiler のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmslTypeMgr.h"
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
  void
  phase1(const AstStatement* stmt,
	 YmslScope* scope);

  /// @brief enum 型の定義を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  ///
  /// stmt は kEnumDecl でなければならない．
  void
  reg_enum(const AstStatement* stmt,
	   YmslScope* scope);

  /// @brief 関数の定義を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  ///
  /// stmt は kFuncDecl でなければならない．
  void
  reg_func(const AstStatement* stmt,
	   YmslScope* scope);

  /// @brief 変数の定義を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  ///
  /// stmt は kVarDecl でなければならない．
  void
  reg_var(const AstStatement* stmt,
	  YmslScope* scope);

  /// @brief 型の参照を解決する．
  /// @param[in] asttype 型を表す構文木
  /// @param[in] scope 現在のスコープ
  ///
  /// 解決できない時には NULL を返す．
  const YmslType*
  resolve_type(const AstType* asttype,
	       YmslScope* scope);

  /// @brief 参照の解決を行う．
  /// @param[in] stmt 文
  /// @param[in] scope 現在のスコープ
  void
  phase2(const AstStatement* stmt,
	 YmslScope* scope);

  /// @brief 式の実体化を行う．
  /// @param[in] ast_expr 式を表す構文木
  /// @param[in] scope 現在のスコープ
  YmslExpr*
  elab_expr(const AstExpr* ast_expr,
	    YmslScope* scope);

  /// @brief 終端式の実体化を行う．
  /// @param[in] ast_leaf 式を表す構文木
  /// @param[in] scope 現在のスコープ
  YmslLeaf*
  elab_leaf(const AstLeaf* ast_leaf,
	    YmslScope* scope);

  /// @brief 変数の参照を解決する．


  /// @brief 変数を生成する．
  /// @param[in] name 名前
  /// @param[in] type 型
  YmslVar*
  new_var(ShString name,
	  const YmslType* type);

  /// @brief 関数を生成する．
  /// @param[in] name 名前
  /// @param[in] type 出力の型
  /// @param[in] input_type_list 入力の型のリスト
  YmslFunction*
  new_function(ShString name,
	       const YmslType* type,
	       const vector<const YmslType*>& input_type_list);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 型を管理するマネージャ
  YmslTypeMgr mTypeMgr;

  // 文とスコープのリスト
  vector<pair<const AstStatement*, YmslScope*> > mStmtList;

  // 変数のリスト
  vector<YmslVar*> mVarList;

  // 関数のリスト
  vector<YmslFunction*> mFuncList;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLCOMPILER_H
