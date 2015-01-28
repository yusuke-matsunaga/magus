#ifndef SETTYPE_H
#define SETTYPE_H

/// @file SetType.h
/// @brief SetType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class SetType SetType.h "SetType.h"
/// @brief set 型を表すクラス
//////////////////////////////////////////////////////////////////////
class SetType :
  public Type
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_type 要素の型
  SetType(const Type* elem_type);

  /// @brief デストラクタ
  virtual
  ~SetType();


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

#endif // SETTYPE_H
