#ifndef YMSLFLOATCONST_H
#define YMSLFLOATCONST_H

/// @file IrFloatConst.h
/// @brief IrFloatConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrFloatConst IrFloatConst.h "IrFloatConst.h"
/// @brief 実数定数を表すクラス
//////////////////////////////////////////////////////////////////////
class IrFloatConst :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  IrFloatConst(double val);

  /// @brief デストラクタ
  virtual
  ~IrFloatConst();


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
