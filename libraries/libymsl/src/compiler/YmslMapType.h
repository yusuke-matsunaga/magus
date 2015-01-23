#ifndef YMSLMAPTYPE_H
#define YMSLMAPTYPE_H

/// @file YmslMapType.h
/// @brief YmslMapType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslMapType YmslMapType.h "YmslMapType.h"
/// @brief map 型を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslMapType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  /// @param[in] key_type キーの型
  /// @param[in] elem_type 要素の型
  YmslMapType(const YmslType* key_type,
	      const YmslType* elem_type);

  /// @brief デストラクタ
  virtual
  ~YmslMapType();


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
  const YmslType*
  key_type() const;

  /// @brief 要素の型を得る．
  ///
  /// array/set/map のみ有効
  virtual
  const YmslType*
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
  const YmslType* mKeyType;

  // 要素の型
  const YmslType* mElemType;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLMAPTYPE_H
