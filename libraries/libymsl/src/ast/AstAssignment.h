#ifndef ASTASSIGNMENT_H
#define ASTASSIGNMENT_H

/// @file AstAssignment.h
/// @brief AstAssignment のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstAssignment AstAssignment.h "AstAssignment.h"
/// @brief 代入文を表す AstStatement
//////////////////////////////////////////////////////////////////////
class AstAssignment :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt_type 文の種類
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  /// @param[in] loc ファイル位置
  AstAssignment(StmtType stmt_type,
		AstExpr* left,
		AstExpr* right,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

  /// @brief 左辺式を返す．
  ///
  /// kAssignment のみ有効
  virtual
  const AstExpr*
  lhs_expr() const;

  /// @brief 式を返す．
  ///
  /// kAssignment,
  /// kDoWhile, kFor, kIf, kWhile, kSwitch
  /// kExprStmt, kReturn, kVarDecl のみ有効
  virtual
  const AstExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文の種類
  StmtType mStmtType;

  // 左辺式
  AstExpr* mLeft;

  // 右辺式
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTASSIGNMENT_H
