#ifndef YMSLASTFLOATTYPE_H
#define YMSLASTFLOATTYPE_H

/// @file YmslAstFloatType.h
/// @brief YmslAstFloatType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstFloatType YmslAstFloatType.h "YmslAstFloatType.h"
/// @brief float 型を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstFloatType :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  YmslAstFloatType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstFloatType();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTFLOATTYPE_H
