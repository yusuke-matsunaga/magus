#ifndef YMSLEXPR_H
#define YMSLEXPR_H

/// @file YmslExpr.h
/// @brief YmslExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslExpr YmslExpr.h "YmslExpr.h"
/// @brief 式を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslExpr
{
public:

  /// @brief コンストラクタ
  YmslExpr();

  /// @brief デストラクタ
  virtual
  ~YmslExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const = 0;

  /// @brief 整数値を得る．
  ///
  /// kIntConst のみ有効
  virtual
  int
  int_val() const;

  /// @brief 実数値を得る．
  ///
  /// kFloatConst のみ有効
  virtual
  double
  float_val() const;

  /// @brief 文字列を得る．
  ///
  /// kStringConst のみ有効
  virtual
  const char*
  string_val() const;

  /// @brief 終端式を返す．
  ///
  /// kLeafExpr の時のみ有効
  virtual
  YmslLeaf*
  leaf() const;

  /// @brief オペランドの数を返す．
  ///
  /// 演算子の時のみ有効
  virtual
  ymuint
  operand_num() const;

  /// @brief オペランドを返す．
  /// @param[in] pos 位置 ( 0 <= pos < operand_num() )
  ///
  /// 演算子の時のみ有効
  virtual
  YmslExpr*
  operand(ymuint pos) const;

  /// @brief 関数本体を返す．
  ///
  /// kFuncCall のみ有効
  virtual
  YmslLeaf*
  func() const;

  /// @brief 関数の引数の数を得る．
  ///
  /// kFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 関数の引数を得る．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kFuncCall のみ有効
  virtual
  YmslExpr*
  arglist_elem(ymuint pos) const;

};

END_NAMESPACE_YM_YMSL


#endif // YMSLEXPR_H
