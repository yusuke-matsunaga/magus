#ifndef ASTUNIOP_H
#define ASTUNIOP_H

/// @file AstUniOp.h
/// @brief AstUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstUniOp AstUniOp.h "AstUniOp.h"
/// @brief 単項演算子を表す Ast
//////////////////////////////////////////////////////////////////////
class AstUniOp :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] op 演算子のタイプ
  /// @param[in] opr オペランド
  /// @param[in] loc ファイル位置
  AstUniOp(ExprType op,
	   AstExpr* opr,
	   const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstUniOp();


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

  // 演算子のタイプ
  ExprType mOp;

  // オペランド
  AstExpr* mOperand;

};

END_NAMESPACE_YM_YMSL

#endif // ASTUNIOP_H
