#ifndef YMSLASTBREAK_H
#define YMSLASTBREAK_H

/// @file YmslAstBreak.h
/// @brief YmslAstBreak のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstBreak YmslAstBreak.h "YmslAstBreak.h"
/// @brief break 文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstBreak :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  YmslAstBreak(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstBreak();


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

#endif // YMSLASTBREAK_H
