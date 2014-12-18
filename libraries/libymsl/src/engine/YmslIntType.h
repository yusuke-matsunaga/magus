#ifndef YMSLINTTYPE_H
#define YMSLINTTYPE_H

/// @file YmslIntType.h
/// @brief YmslIntType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslValueType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslIntType YmslIntType.h "YmslIntType.h"
/// @brief int 型を表す YmslValueType
//////////////////////////////////////////////////////////////////////
class YmslIntType :
  public YmslValueType
{
public:

  /// @brief コンストラクタ
  YmslIntType();

  /// @brief デストラクタ
  virtual
  ~YmslIntType();


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

#endif // YMSLINTTYPE_H
