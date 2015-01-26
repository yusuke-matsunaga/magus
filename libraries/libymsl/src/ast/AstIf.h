#ifndef ASTIF_H
#define ASTIF_H

/// @file AstIf.h
/// @brief AstIf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIf AstIf.h "AstIf.h"
/// @brief if 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstIf :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 条件式
  /// @param[in] then_stmt 条件が成り立った時実行される文のリスト
  /// @param[in] else_stmt 条件が成り立たなかった時実行される文
  /// @param[in] loc ファイル位置
  AstIf(AstExpr* expr,
	AstStatement* then_stmt,
	AstStatement* else_stmt,
	const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

  /// @brief 式を返す．
  ///
  /// kAssignment,
  /// kDoWhile, kFor, kIf, kWhile, kSwitch
  /// kExprStmt, kReturn, kVarDecl のみ有効
  virtual
  const AstExpr*
  expr() const;

  /// @brief 本体の文を返す．
  ///
  /// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
  virtual
  const AstStatement*
  stmt() const;

  /// @brief else 節を得る．
  ///
  /// kIf のみ有効
  virtual
  const AstStatement*
  else_stmt() const;

#if 0
  /// @brief 要素の生成と関数以外の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  /// @param[in] type_mgr 型マネージャ
  virtual
  void
  phase1(YmslScope* parent_scope,
	 YmslTypeMgr* type_mgr);

  /// @brief 関数の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  virtual
  void
  phase2(YmslScope* parent_scope);

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size();

  /// @brief 命令コードを生成する．
  /// @param[in] driver ドライバ
  /// @param[in] code_list 命令コードの格納先
  /// @param[inout] addr 命令コードの現在のアドレス
  ///
  /// addr の値は更新される．
  virtual
  void
  compile(YmslDriver& driver,
	  YmslCodeList& code_list,
	  Ymsl_INT& addr);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;
#endif


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式
  AstExpr* mExpr;

  // then 節
  AstStatement* mStmt;

  // else 節
  AstStatement* mElseStmt;

};

END_NAMESPACE_YM_YMSL


#endif // ASTIF_H
