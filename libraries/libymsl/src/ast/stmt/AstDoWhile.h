#ifndef ASTDOWHILE_H
#define ASTDOWHILE_H

/// @file AstDoWhile.h
/// @brief AstDoWhile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstDoWhile AstDoWhile.h "AstDoWhile.h"
/// @brief for 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstDoWhile :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt 本体の文
  /// @param[in] cond 条件式
  /// @param[in] loc ファイル位置
  AstDoWhile(AstStatement* stmt,
	     AstExpr* cond,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstDoWhile();


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


#endif // ASTDOWHILE_H
