#ifndef IREXPR_H
#define IREXPR_H

/// @file IrExpr.h
/// @brief IrExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrExpr IrExpr.h "IrExpr.h"
/// @brief 式を表すクラス
//////////////////////////////////////////////////////////////////////
class IrExpr
{
public:

  /// @brief コンストラクタ
  IrExpr();

  /// @brief デストラクタ
  virtual
  ~IrExpr();


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

  /// @brief 変数を返す．
  ///
  /// kSymbolExpr のみ有効
  virtual
  Var*
  var() const;

  /// @brief 本体の式を返す．
  ///
  /// kMemberRef, kArrayRef のみ有効
  virtual
  IrExpr*
  body() const;

  /// @brief メンバのオフセットを返す．
  ///
  /// kMemberRef のみ有効
  virtual
  ymuint
  member_offset() const;

  /// @biref インデックスの式を返す．
  ///
  /// kArrayRef のみ有効
  virtual
  IrExpr*
  array_index() const;

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
  IrExpr*
  operand(ymuint pos) const;

  /// @brief 関数本体を返す．
  ///
  /// kFuncCall のみ有効
  virtual
  IrExpr*
  func_expr() const;

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
  IrExpr*
  arglist_elem(ymuint pos) const;

  /// @brief スコープを返す．
  ///
  /// kScopeExpr のみ有効
  virtual
  Scope*
  scope() const;

  /// @brief enum 型を返す．
  ///
  /// kEnumExpr のみ有効．
  virtual
  const Type*
  enum_type() const;

  /// @brief 関数を返す．
  ///
  /// kFuncExpr のみ有効
  virtual
  Function*
  function() const;

};

END_NAMESPACE_YM_YMSL


#endif // IREXPR_H
