#ifndef ASTFOR_H
#define ASTFOR_H

/// @file AstFor.h
/// @brief AstFor のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFor AstFor.h "AstFor.h"
/// @brief for 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstFor :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] init 初期化文
  /// @param[in] cond 条件式
  /// @param[in] next 増加文
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstFor(AstStatement* init,
	 AstExpr* cond,
	 AstStatement* next,
	 AstStatement* stmt,
	 const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFor();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

  /// @brief 条件式を返す．
  ///
  /// kDoWhile, kFor, kIf, kWhile, kSwitch のみ有効
  virtual
  const AstExpr*
  expr() const;

  /// @brief 初期化文を返す．
  ///
  /// kFor のみ有効
  virtual
  const AstStatement*
  init_stmt() const;

  /// @brief 増加文を返す．
  ///
  /// kFor のみ有効
  virtual
  const AstStatement*
  next_stmt() const;

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

  // 初期化文
  AstStatement* mInit;

  // 条件
  AstExpr* mExpr;

  // 増加文
  AstStatement* mNext;

  // 本体の文
  AstStatement* mStmt;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLASTFOR_H
