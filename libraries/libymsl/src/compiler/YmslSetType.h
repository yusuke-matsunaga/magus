#ifndef YMSLSETTYPE_H
#define YMSLSETTYPE_H

/// @file YmslSetType.h
/// @brief YmslSetType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslSetType YmslSetType.h "YmslSetType.h"
/// @brief set 型を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslSetType :
  public YmslType
{
public:

  /// @brief コンストラクタ
  /// @param[in] elem_type 要素の型
  YmslSetType(const YmslType* elem_type);

  /// @brief デストラクタ
  virtual
  ~YmslSetType();


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

  // 要素の型
  const YmslType* mElemType;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLSETTYPE_H
