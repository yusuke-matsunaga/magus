#ifndef ASTRETURN_H
#define ASTRETURN_H

/// @file AstReturn.h
/// @brief AstReturn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstReturn AstReturn.h "AstReturn.h"
/// @brief return 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstReturn :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 値
  /// @param[in] loc ファイル位置
  AstReturn(AstExpr* expr,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstReturn();


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
  /// kExprStmt, kReturn のみ有効
  virtual
  const AstExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTRETURN_H
