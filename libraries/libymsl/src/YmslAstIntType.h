#ifndef YMSLASTINTTYPE_H
#define YMSLASTINTTYPE_H

/// @file YmslAstIntType.h
/// @brief YmslAstIntType のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstIntType YmslAstIntType.h "YmslAstIntType.h"
/// @brief int 型を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstIntType :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  YmslAstIntType(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstIntType();


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
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLASTINTTYPE_H
