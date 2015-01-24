#ifndef YMSLFLOATCONST_H
#define YMSLFLOATCONST_H

/// @file YmslFloatConst.h
/// @brief YmslFloatConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslFloatConst YmslFloatConst.h "YmslFloatConst.h"
/// @brief 実数定数を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslFloatConst :
  public YmslExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  YmslFloatConst(double val);

  /// @brief デストラクタ
  virtual
  ~YmslFloatConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 実数値を返す．
  ///
  /// kFloatConst の時のみ有効
  virtual
  double
  float_val() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mVal;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLFLOATCONST_H
