#ifndef ASTINTTYPE_H
#define ASTINTTYPE_H

/// @file AstIntType.h
/// @brief AstIntType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstValueType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIntType AstIntType.h "AstIntType.h"
/// @brief int 型を表す AstValueType
//////////////////////////////////////////////////////////////////////
class AstIntType :
  public AstValueType
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstIntType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIntType();


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

#endif // ASTINTTYPE_H
