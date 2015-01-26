#ifndef YMSLSTRINGCONST_H
#define YMSLSTRINGCONST_H

/// @file IrStringConst.h
/// @brief IrStringConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrStringConst IrStringConst.h "IrStringConst.h"
/// @brief 文字列定数を表すクラス
//////////////////////////////////////////////////////////////////////
class IrStringConst :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] val 値
  IrStringConst(const char* val);

  /// @brief デストラクタ
  virtual
  ~IrStringConst();


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
