#ifndef ASTVAREXPR_H
#define ASTVAREXPR_H

/// @file AstVarExpr.h
/// @brief AstVarExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstVarExpr AstVarExpr.h "AstVarExpr.h"
/// @brief 変数を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstVarExpr :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] var 変数
  /// @param[in] loc ファイル位置
  AstVarExpr(AstVarDecl* var,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstVarExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 変数
  AstVarDecl* mVar;

};

END_NAMESPACE_YM_YMSL


#endif // ASTVAREXPR_H
