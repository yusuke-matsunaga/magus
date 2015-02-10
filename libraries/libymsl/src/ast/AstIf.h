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
  Type
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
