#ifndef ASTEXPRSTMT_H
#define ASTEXPRSTMT_H

/// @file AstWhile.h
/// @brief AstWhile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstExprStmt AstExprStmt.h "AstExprStmt.h"
/// @brief 式文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstExprStmt :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 式
  /// @param[in] loc ファイル位置
  AstExprStmt(AstExpr* expr,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstExprStmt();


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
  /// kExprStmt のみ有効
  virtual
  const AstExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL


#endif // ASTEXPRSTMT_H
