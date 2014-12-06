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
  AstExprStmt(AstExpr* expr);

  /// @brief デストラクタ
  virtual
  ~AstExprStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL


#endif // ASTEXPRSTMT_H
