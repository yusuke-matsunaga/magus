#ifndef ASTFLOATCONST_H
#define ASTFLOATCONST_H

/// @file AstFloatConst.h
/// @brief AstFloatConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFloatConst AstFloatConst.h "AstFloatConst.h"
/// @brief 浮動小数点型の定数を表す Ast
//////////////////////////////////////////////////////////////////////
class AstFloatConst :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  /// @param[in] loc ファイル位置
  AstFloatConst(double val,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFloatConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  expr_type() const;

  /// @brief 浮動小数点値を返す．
  ///
  /// kFloatConst のみ有効
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

#endif // YMSLASTFLOATCONST_H
