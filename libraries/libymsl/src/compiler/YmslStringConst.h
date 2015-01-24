#ifndef YMSLSTRINGCONST_H
#define YMSLSTRINGCONST_H

/// @file YmslStringConst.h
/// @brief YmslStringConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslStringConst YmslStringConst.h "YmslStringConst.h"
/// @brief 文字列定数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslStringConst :
  public YmslExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  YmslStringConst(const char* val);

  /// @brief デストラクタ
  virtual
  ~YmslStringConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 文字列を返す．
  ///
  /// kStringConst の時のみ有効
  virtual
  const char*
  string_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  const char* mVal;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLSTRINGCONST_H
