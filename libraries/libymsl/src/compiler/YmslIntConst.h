#ifndef YMSLINTCONST_H
#define YMSLINTCONST_H

/// @file YmslIntConst.h
/// @brief YmslIntConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslIntConst YmslIntConst.h "YmslIntConst.h"
/// @brief 整数定数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslIntConst :
  public YmslExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  YmslIntConst(int val);

  /// @brief デストラクタ
  virtual
  ~YmslIntConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 整数値を返す．
  ///
  /// kIntConst の時のみ有効
  virtual
  int
  int_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  int mVal;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLINTCONST_H
