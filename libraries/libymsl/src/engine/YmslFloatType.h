#ifndef YMSLFLOATTYPE_H
#define YMSLFLOATTYPE_H

/// @file YmslFloatType.h
/// @brief YmslFloatType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslValueType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslFloatType YmslFloatType.h "YmslFloatType.h"
/// @brief float 型を表す YmslValueType
//////////////////////////////////////////////////////////////////////
class YmslFloatType :
  public YmslValueType
{
public:

  /// @brief コンストラクタ
  YmslFloatType();

  /// @brief デストラクタ
  virtual
  ~YmslFloatType();


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

#endif // YMSLFLOATTYPE_H
