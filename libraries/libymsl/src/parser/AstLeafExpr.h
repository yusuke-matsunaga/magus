#ifndef ASTLEAFEXPR_H
#define ASTLEAFEXPR_H

/// @file AstLeafExpr.h
/// @brief AstLeafExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstLeafExpr AstLeafExpr.h "AstLeafExpr.h"
/// @brief 終端式を表すクラス
//////////////////////////////////////////////////////////////////////
class AstLeafExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  AstLeafExpr(AstLeaf* leaf);

  /// @breif デストラクタ
  virtual
  ~AstLeafExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  ExprType
  expr_type() const;

  /// @brief 終端を返す．
  ///
  /// kLeafExpr のみ有効
  virtual
  const AstLeaf*
  leaf() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 終端
  AstLeaf* mLeaf;

};

END_NAMESPACE_YM_YMSL

#endif // ASTLEAFEXPR_H
