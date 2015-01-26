#ifndef YMSLSETTYPE_H
#define YMSLSETTYPE_H

/// @file IrSetType.h
/// @brief IrSetType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrSetType IrSetType.h "IrSetType.h"
/// @brief set 型を表すクラス
//////////////////////////////////////////////////////////////////////
class IrSetType :
  public IrType
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_type 要素の型
  IrSetType(const IrType* elem_type);

  /// @brief デストラクタ
  virtual
  ~IrSetType();


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

#endif // YMSLSETTYPE_H
