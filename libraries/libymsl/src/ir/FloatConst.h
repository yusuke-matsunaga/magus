#ifndef FLOATCONST_H
#define FLOATCONST_H

/// @file FloatConst.h
/// @brief FloatConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "ConstVal.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class FloatConst FloatConst.h "FloatConst.h"
/// @brief 実数定数を表すノード
//////////////////////////////////////////////////////////////////////
class FloatConst :
  public ConstVal
{
public:

  /// @brief コンストラクタ
  /// @param[in] type 型
  /// @param[in] val 値
  FloatConst(const Type* type,
	     double val);

  /// @brief デストラクタ
  virtual
  ~FloatConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 実数値を返す．
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

#endif // FLOATCONST_H
