#ifndef YMSLASTCONTINUE_H
#define YMSLASTCONTINUE_H

/// @file YmslAstContinue.h
/// @brief YmslAstContinue のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmslAstImpl.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslAstContinue YmslAstContinue.h "YmslAstContinue.h"
/// @brief continue 文を表す YmslAst
//////////////////////////////////////////////////////////////////////
class YmslAstContinue :
  public YmslAstImpl
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  YmslAstContinue(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~YmslAstContinue();


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

#endif // YMSLASTCONTINUE_H
