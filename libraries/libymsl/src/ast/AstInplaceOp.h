#ifndef ASTINPLACEOP_H
#define ASTINPLACEOP_H

/// @file AstInplaceOp.h
/// @brief AstInplaceOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstInplaceOp AstInplaceOp.h "AstInplaceOp.h"
/// @brief 代入文を表す AstStatement
//////////////////////////////////////////////////////////////////////
class AstInplaceOp :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  /// @param[in] loc ファイル位置
  AstInplaceOp(OpCode opcode,
	       AstExpr* left,
	       AstExpr* right,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstInplaceOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

  /// @brief オペコードを返す．
  ///
  /// kInplaceOp のみ有効
  virtual
  OpCode
  opcode() const;

  /// @brief 左辺式を返す．
  ///
  /// kInplaceOp のみ有効
  virtual
  const AstExpr*
  lhs_expr() const;

  /// @brief 式を返す．
  ///
  /// kInplaceOp,
  /// kDoWhile, kFor, kIf, kWhile, kSwitch
  /// kExprStmt, kReturn, kVarDecl のみ有効
  virtual
  const AstExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペコード
  OpCode mOpCode;

  // 左辺式
  AstExpr* mLeft;

  // 右辺式
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTINPLACEOP_H
