#ifndef YMSLSTRINGTYPE_H
#define YMSLSTRINGTYPE_H

/// @file YmslStringType.h
/// @brief YmslStringType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslValueType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslStringType YmslStringType.h "YmslStringType.h"
/// @brief string 型を表す YmslValueType
//////////////////////////////////////////////////////////////////////
class YmslStringType :
  public YmslValueType
{
public:

  /// @brief コンストラクタ
  YmslStringType();

  /// @brief デストラクタ
  virtual
  ~YmslStringType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  ValueType
  value_type() const;

  /// @brief 値を表す文字列を返す．
  virtual
  string
  str() const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTSTRINGTYPE_H
