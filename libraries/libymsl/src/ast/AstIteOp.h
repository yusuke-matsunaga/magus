#ifndef ASTITEOP_H
#define ASTITEOP_H

/// @file AstIteOp.h
/// @brief AstIteOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @ckass AstIteOp AstIteOp.h "AstIteOp.h"
/// @brief ITE演算子を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstIteOp :
  public AstExpr
{
public:

  /// @breif コンストラクタ
  /// @param[in] opr1, opr2, opr3 オペランド
  AstIteOp(AstExpr* opr1,
	   AstExpr* opr2,
	   AstExpr* opr3);

  /// @brief デストラクタ
  virtual
  ~AstIteOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief オペランド数を返す．
  ///
  /// 演算子のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num()
  ///
  /// 演算子のみ有効
  virtual
  const AstExpr*
  operand(ymuint pos) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // オペランド
  AstExpr* mOpr[3];

};

END_NAMESPACE_YM_YMSL

#endif // ASTITEOP_H
