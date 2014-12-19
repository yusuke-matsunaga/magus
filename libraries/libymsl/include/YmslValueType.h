#ifndef YMSLVALUETYPE_H
#define YMSLVALUETYPE_H

/// @file AstValueType.h
/// @brief AstValueType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "ymsl_int.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslValueType YmslValueType.h "YmslValueType.h"
/// @brief 値の型を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslValueType
{
public:

  /// @brief デストラクタ
  virtual
  ~YmslValueType() { }


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  ValueType
  value_type() const = 0;

  /// @brief 値を表す文字列を返す．
  virtual
  string
  str() const = 0;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLVALUETYPE_H
