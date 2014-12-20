#ifndef ASTVOIDTYPE_H
#define ASTVOIDTYPE_H

/// @file AstVoidType.h
/// @brief AstVoidType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstValueType.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstVoidType AstVoidType.h "AstVoidType.h"
/// @brief void 型を表す AstValueType
//////////////////////////////////////////////////////////////////////
class AstVoidType :
  public AstValueType
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstVoidType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstVoidType();


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

#endif // ASTVOIDTYPE_H
