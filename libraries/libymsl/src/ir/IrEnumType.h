#ifndef YMSLENUMTYPE_H
#define YMSLENUMTYPE_H

/// @file IrEnumType.h
/// @brief IrEnumType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrNamedType.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrEnumType IrEnumType.h "IrEnumType.h"
/// @brief enum 型を表すクラス
//////////////////////////////////////////////////////////////////////
class IrEnumType :
  public IrNamedType
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 名前
  /// @param[in] elem_list 要素名と値のリスト
  IrEnumType(ShString name,
	       const vector<pair<ShString, int> >& elem_list);

  /// @brief デストラクタ
  virtual
  ~IrEnumType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief 列挙型の数を得る．
  ///
  /// enum のみ有効
  virtual
  ymuint
  enum_num() const;

  /// @brief 列挙型の定数を得る．
  /// @param[in] index インデックス ( 0 <= index < enum_num() )
  ///
  /// enum のみ有効
  virtual
  const IrEnumConst*
  enum_elem(ymuint index) const;

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
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 要素数
  ymuint mElemNum;

  // 要素の配列
  IrEnumConst* mElemArray;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLENUMTYPE_H
