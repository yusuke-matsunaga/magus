#ifndef TYPE_H
#define TYPE_H

/// @file AstValueType.h
/// @brief AstValueType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"
#include "YmUtils/HashFunc.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class Type Type.h "Type.h"
/// @brief 値の型を表すクラス
//////////////////////////////////////////////////////////////////////
class Type
{
  friend class TypeMgr;

public:

  /// @brief コンストラクタ
  /// @param[in] type_id タイプID
  Type(TypeId type_id);

  /// @brief デストラクタ
  virtual
  ~Type();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  TypeId
  type_id() const;

  /// @brief 指定された型にキャスト可能な場合に true を返す．
  /// @param[in] type 指定された型
  virtual
  bool
  castable_to(const Type* type) const = 0;

  /// @brief 型名を得る．
  ///
  /// enum のみ有効
  virtual
  ShString
  type_name() const;

  /// @brief ID番号を得る
  ///
  /// おもにハッシュ関数用
  ymuint
  id() const;

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

  /// @brief 関数の出力の型を返す．
  ///
  /// function のみ有効
  virtual
  const Type*
  function_output_type() const;

  /// @brief 関数の入力数を返す．
  ///
  /// function のみ有効
  virtual
  ymuint
  function_input_num() const;

  /// @brief 関数の入力の型を返す．
  /// @param[in] pos 入力番号 ( 0 <= pos < function_input_num() )
  ///
  /// function のみ有効
  virtual
  const Type*
  function_input_type(ymuint pos) const;

  /// @brief 列挙型の数を得る．
  ///
  /// enum のみ有効
  virtual
  ymuint
  enum_num() const;

  /// @brief 列挙型の定数値を得る．
  /// @param[in] index インデックス ( 0 <= index < enum_num() )
  ///
  /// enum のみ有効
  virtual
  Ymsl_INT
  enum_elem_val(ymuint index) const;

  /// @brief 列挙型の定数名を得る．
  /// @param[in] index インデックス ( 0 <= index < enum_num() )
  ///
  /// enum のみ有効
  virtual
  ShString
  enum_elem_name(ymuint index) const;

  /// @brief 列挙型のインデックスを得る．
  /// @param[in] name 列挙型の定数の名前
  ///
  /// enum のみ有効
  /// 該当する名前がなければ -1 を返す．
  virtual
  int
  enum_index(ShString name) const;

  /// @brief 内容を出力する．
  /// @param[in] s 出力先のストリーム
  virtual
  void
  print(ostream& s) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // タイプID
  TypeId mTypeId;

  // ID番号
  ymuint mId;

  // ハッシュ表中で次の要素を指すリンクポインタ
  Type* mLink;

};

// @brief ID番号を得る
//
// おもにハッシュ関数用
inline
ymuint
Type::id() const
{
  return mId;
}

END_NAMESPACE_YM_YMSL


BEGIN_NAMESPACE_YM

//////////////////////////////////////////////////////////////////////
// HashFunc<const Type*> の特殊化
//////////////////////////////////////////////////////////////////////
template<>
struct
HashFunc<const nsYmsl::Type*>
{
  ymuint
  operator()(const nsYmsl::Type* key) const
  {
    return key->id();
  }
};

END_NAMESPACE_YM

#endif // TYPE_H
