#ifndef ASTBINOP_H
#define ASTBINOP_H

/// @file AstBinOp.h
/// @brief AstBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstOp.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @ckass AstBinOp AstBinOp.h "AstBinOp.h"
/// @brief 二項演算子を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstBinOp :
  public AstOp
{
public:

  /// @breif コンストラクタ
  /// @param[in] opcode オペコード
  /// @param[in] left, right オペランド
  AstBinOp(OpCode opcode,
	   AstExpr* left,
	   AstExpr* right);

  /// @brief デストラクタ
  virtual
  ~AstBinOp();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
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

  // 左オペランド
  AstExpr* mLeft;

  // 右オペランド
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBINOP_H
