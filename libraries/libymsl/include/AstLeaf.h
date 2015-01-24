#ifndef ASTLEAF_H
#define ASTLEAF_H

/// @file AstLeaf.h
/// @brief AstLeaf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstLeaf AstLeaf.h "AstLeaf.h"
/// @brief 終端を表す AST
//////////////////////////////////////////////////////////////////////
class AstLeaf :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstLeaf(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstLeaf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  LeafType
  leaf_type() const = 0;

  /// @brief シンボルを返す．
  ///
  /// kSymbolExpr, kMemberRef のみ有効
  virtual
  const AstSymbol*
  symbol() const;

  /// @brief 本体を返す．
  ///
  /// kArrayRef, kMemberRef, FuncCall のみ有効
  virtual
  const AstLeaf*
  body() const;

  /// @brief 配列のインデックスを返す．
  ///
  /// kArrayRef のみ有効
  virtual
  const AstExpr*
  index() const;

  /// @brief 引数の数を返す．
  ///
  /// kFuncCall のみ有効
  virtual
  ymuint
  arglist_num() const;

  /// @brief 引数を返す．
  /// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
  ///
  /// kFuncCall のみ有効
  virtual
  const AstExpr*
  arglist_elem(ymuint pos) const;

  /// @brief 整数値を返す．
  ///
  /// kIntConst のみ有効
  virtual
  int
  int_val() const;

  /// @brief 実数値を返す．
  ///
  /// kFloatConst のみ有効
  virtual
  double
  float_val() const;

  /// @brief 文字列値を返す．
  ///
  /// kStringConst のみ有効
  virtual
  const char*
  string_val() const;

};

END_NAMESPACE_YM_YMSL

#endif // ASTLEAF_H
