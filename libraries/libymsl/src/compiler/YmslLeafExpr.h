#ifndef YMSLLEAFEXPR_H
#define YMSLLEAFEXPR_H

/// @file YmslLeafExpr.h
/// @brief YmslLeafExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslLeafExpr YmslLeafExpr.h "YmslLeafExpr.h"
/// @brief 終端式を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslLeafExpr :
  public YmslExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] leaf 終端
  YmslLeafExpr(YmslLeaf* leaf);

  /// @brief デストラクタ
  virtual
  ~YmslLeafExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 終端式を返す．
  ///
  /// kLeafExpr の時のみ有効
  virtual
  YmslLeaf*
  leaf() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 終端
  YmslLeaf* mLeaf;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLLEAFEXPR_H
