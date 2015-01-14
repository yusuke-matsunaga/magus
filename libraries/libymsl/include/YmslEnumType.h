#ifndef YMSLENUMTYPE_H
#define YMSLENUMTYPE_H

/// @file YmslEnumType.h
/// @brief YmslEnum のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslEnumConst YmslEnumType.h "YmslEnumType.h"
/// @brief enum 定数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslEnumConst
{
  friend class YmslEnumType;

public:

  /// @brief コンストラクタ
  YmslEnumConst();

  /// @brief デストラクタ
  ~YmslEnumConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief enum 型を返す．
  YmslEnumType*
  parent() const;

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief 値を返す．
  ymuint
  val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 親の enum 型
  YmslEnumType* mParent;

  // 名前
  ShString mName;

  // 値
  ymuint mVal;

};


//////////////////////////////////////////////////////////////////////
/// @class YmslEnumType YmslEnumType.h "YmslEnumType.h"
/// @brief enum 型を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslEnumType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  /// @param[in] id ID番号
  /// @param[in] name 名前
  /// @param[in] elem_list 要素のリスト
  YmslEnumType(ymuint id,
	       ShString name,
	       const vector<ShString>& elem_list);

  /// @brief デストラクタ
  virtual
  ~YmslEnumType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  TypeId
  type_id() const;

  /// @brief 型名を得る．
  ///
  /// enum のみ有効
  virtual
  ShString
  type_name() const;

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
  const YmslEnumConst*
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

  // 名前
  ShString mName;

  // 要素数
  ymuint mElemNum;

  // 要素の配列
  YmslEnumConst* mElemArray;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLENUMTYPE_H
