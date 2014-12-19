#ifndef ASTSTRINGTYPE_H
#define ASTSTRINGTYPE_H

/// @file AstStringType.h
/// @brief AstStringType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstValueType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstStringType AstStringType.h "AstStringType.h"
/// @brief string 型を表す AstValueType
//////////////////////////////////////////////////////////////////////
class AstStringType :
  public AstValueType
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstStringType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstStringType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 単純型の場合に true を返す．
  virtual
  bool
  simple_type() const;

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
