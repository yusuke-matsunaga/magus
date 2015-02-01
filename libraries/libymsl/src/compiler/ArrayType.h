#ifndef ARRAYTYPE_H
#define ARRAYTYPE_H

/// @file IrArrayType.h
/// @brief IrArrayType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class ArrayType ArrayType.h "ArrayType.h"
/// @brief array 型を表すクラス
//////////////////////////////////////////////////////////////////////
class ArrayType :
  public Type
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_type 要素の型
  ArrayType(const Type* elem_type);

  /// @brief デストラクタ
  virtual
  ~ArrayType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 指定された型にキャスト可能な場合に true を返す．
  /// @param[in] type 指定された型
  virtual
  bool
  castable_to(const Type* type) const;

  /// @brief 要素の型を得る．
  ///
  /// array/set/map のみ有効
  virtual
  const Type*
  elem_type() const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素の型
  const Type* mElemType;

};

END_NAMESPACE_YM_YMSL

#endif // ARRAYTYPE_H
