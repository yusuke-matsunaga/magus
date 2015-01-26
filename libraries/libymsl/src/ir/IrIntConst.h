#ifndef YMSLINTCONST_H
#define YMSLINTCONST_H

/// @file IrIntConst.h
/// @brief IrIntConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrIntConst IrIntConst.h "IrIntConst.h"
/// @brief 整数定数を表すクラス
//////////////////////////////////////////////////////////////////////
class IrIntConst :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  IrIntConst(int val);

  /// @brief デストラクタ
  virtual
  ~IrIntConst();


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
