#ifndef ASTVALUETYPE_H
#define ASTVALUETYPE_H

/// @file AstValueType.h
/// @brief AstValueType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @brief 値の型
//////////////////////////////////////////////////////////////////////
enum ValueType {
  kIntType,
  kFloatType,
  kStringType,
  kUserType
};


//////////////////////////////////////////////////////////////////////
/// @class AstValueType AstValueType.h "AstValueType.h"
/// @brief 値の型を表すクラス
//////////////////////////////////////////////////////////////////////
class AstValueType :
  public Ast
{
public:

  /// @brief コンストラクタ
  AstValueType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstValueType();


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


#endif // ASTVALUETYPE_H
