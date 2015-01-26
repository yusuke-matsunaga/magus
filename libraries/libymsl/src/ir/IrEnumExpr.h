#ifndef YMSLENUMEXPR_H
#define YMSLENUMEXPR_H

/// @file IrEnumExpr.h
/// @brief IrEnumExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class IrEnumExpr IrEnumExpr.h "IrEnumExpr.h"
/// @brief enum 型を表す IrExpr
//////////////////////////////////////////////////////////////////////
class IrEnumExpr :
  public IrExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] enum_type enum 型
  IrEnumExpr(const IrType* enum_type);

  /// @brief デストラクタ
  virtual
  ~IrEnumExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief enum 型を返す．
  ///
  /// kEnumExpr のみ有効
  virtual
  const IrType*
  enum_type() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // enum 型
  const IrType* mEnumType;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLENUMEXPR_H
