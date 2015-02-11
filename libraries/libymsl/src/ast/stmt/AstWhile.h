#ifndef ASTWHILE_H
#define ASTWHILE_H

/// @file AstWhile.h
/// @brief AstWhile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstWhile AstWhile.h "AstWhile.h"
/// @brief while 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstWhile :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] cond 条件式
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstWhile(AstExpr* cond,
	   AstStatement* stmt,
	   const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstWhile();


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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  AstExpr* mExpr;

  // 本体の文
  AstStatement* mStmt;

};

END_NAMESPACE_YM_YMSL


#endif // ASTWHILE_H
