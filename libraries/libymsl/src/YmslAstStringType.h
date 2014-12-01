#ifndef YMSLASTSTRINGTYPE_H
#define YMSLASTSTRINGTYPE_H

/// @file YmslAstStringType.h
/// @brief YmslAstStringType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstStringType YmslAstStringType.h "YmslAstStringType.h"
/// @brief string 型を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstStringType :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  YmslAstStringType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstStringType();


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

#endif // YMSLASTSTRINGTYPE_H
