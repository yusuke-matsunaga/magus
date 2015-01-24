#ifndef YMSLLEAF_H
#define YMSLLEAF_H

/// @file YmslLeaf.h
/// @brief YmslLeaf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslLeaf YmslLeaf.h "YmslLeaf.h"
/// @brief 終端式を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslLeaf
{
public:

  /// @brief コンストラクタ
  YmslLeaf();

  /// @brief デストラクタ
  virtual
  ~YmslLeaf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を返す．
  virtual
  LeafType
  leaf_type() const = 0;

  /// @brief シンボル名を返す．

  /// @brief 本体を返す．
  ///
  /// kMemberRef, kArrayRef, kFuncCall のみ有効
  virtual
  YmslLeaf*
  body() const;

  /// @brief メンバのオフセットを得る．
  ///
  /// kMemberRef のみ有効
  virtual
  ymuint
  member_offset() const;

  /// @brief 配列のインデックスを得る．
  ///
  /// kArrayRef のみ有効
  virtual
  YmslExpr*
  array_index() const;

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

};

END_NAMESPACE_YM_YMSL

#endif // YMSLLEAF_H
