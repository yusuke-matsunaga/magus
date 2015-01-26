#ifndef YMSLARRAYTYPE_H
#define YMSLARRAYTYPE_H

/// @file IrArrayType.h
/// @brief IrArrayType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrArrayType IrArrayType.h "IrArrayType.h"
/// @brief array 型を表すクラス
//////////////////////////////////////////////////////////////////////
class IrArrayType :
  public IrType
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_type 要素の型
  IrArrayType(const IrType* elem_type);

  /// @brief デストラクタ
  virtual
  ~IrArrayType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief 要素の型を得る．
  ///
  /// array/set/map のみ有効
  virtual
  const IrType*
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
  const IrType* mElemType;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLARRAYTYPE_H
