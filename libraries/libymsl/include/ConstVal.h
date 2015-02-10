#ifndef CONSTVAL_H
#define CONSTVAL_H

/// @file ConstVal.h
/// @brief ConstVal のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class ConstVal ConstVal.h "ConstVal.h"
/// @brief 定数値を表すクラス
//////////////////////////////////////////////////////////////////////
class ConstVal
{
public:

  /// @brief コンストラクタ
  /// @param[in] value_type 値の型
  ConstVal(const Type* value_type);

  /// @brief デストラクタ
  virtual
  ~ConstVal();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 値の型を返す．
  const Type*
  value_type() const;

  /// @brief ブール値を返す．
  ///
  /// value_type() が boolean_type の時有効
  virtual
  bool
  boolean_val() const;

  /// @brief 整数値を返す．
  ///
  /// value_type() が int_type の時有効
  virtual
  int
  int_val() const;

  /// @brief 実数値を返す．
  ///
  /// value_type() が float_type の時有効
  virtual
  double
  float_val() const;

  /// @brief 文字列を返す．
  ///
  /// value_type() が string_type の時有効
  virtual
  const char*
  string_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値の型
  const Type* mValueType;

};

END_NAMESPACE_YM_YMSL

#endif // CONSTVAL_H
