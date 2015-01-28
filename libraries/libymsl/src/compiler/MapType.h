#ifndef MAPTYPE_H
#define MAPTYPE_H

/// @file MapType.h
/// @brief MapType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class MapType MapType.h "MapType.h"
/// @brief map 型を表すクラス
//////////////////////////////////////////////////////////////////////
class MapType :
  public Type
{
public:

  /// @brief コンストラクタ
  /// @param[in] key_type キーの型
  /// @param[in] elem_type 要素の型
  MapType(const Type* key_type,
	  const Type* elem_type);

  /// @brief デストラクタ
  virtual
  ~MapType();


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
  const Type*
  key_type() const;

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

  // キーの型
  const Type* mKeyType;

  // 要素の型
  const Type* mElemType;

};

END_NAMESPACE_YM_YMSL

#endif // MAPTYPE_H
