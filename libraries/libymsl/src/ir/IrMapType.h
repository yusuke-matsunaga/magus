#ifndef YMSLMAPTYPE_H
#define YMSLMAPTYPE_H

/// @file IrMapType.h
/// @brief IrMapType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrMapType IrMapType.h "IrMapType.h"
/// @brief map 型を表すクラス
//////////////////////////////////////////////////////////////////////
class IrMapType :
  public IrType
{
public:

  /// @brief コンストラクタ
  /// @param[in] key_type キーの型
  /// @param[in] elem_type 要素の型
  IrMapType(const IrType* key_type,
	      const IrType* elem_type);

  /// @brief デストラクタ
  virtual
  ~IrMapType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief キーの型を得る．
  ///
  /// map のみ有効
  virtual
  const IrType*
  key_type() const;

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

  // キーの型
  const IrType* mKeyType;

  // 要素の型
  const IrType* mElemType;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLMAPTYPE_H
