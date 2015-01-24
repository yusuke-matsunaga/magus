#ifndef YMSLVAREXPR_H
#define YMSLVAREXPR_H

/// @file YmslVarExpr.h
/// @brief YmslVarExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslLeaf.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class YmslVarExpr YmslVarExpr.h "YmslVarExpr.h"
/// @brief 変数式を表すクラス
//////////////////////////////////////////////////////////////////////
class YmslVarExpr :
  public YmslLeaf
{
public:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  YmslVarExpr(YmslVar* var);

  /// @brief デストラクタ
  virtual
  ~YmslVarExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  LeafType
  leaf_type() const;

  /// @brief 変数を返す．
  ///
  /// kPrimary の時のみ有効
  virtual
  YmslVar*
  var() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  YmslVar* mVar;

};

END_NAMESPACE_YM_YMSL

#endif // YMSLVAREXPR_H
