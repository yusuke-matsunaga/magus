#ifndef ASTBINOP_H
#define ASTBINOP_H

/// @file AstBinOp.h
/// @brief AstBinOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @ckass AstBinOp AstBinOp.h "AstBinOp.h"
/// @brief 二項演算子を表す AstExpr
//////////////////////////////////////////////////////////////////////
class AstBinOp :
  public AstExpr
{
public:

  /// @breif コンストラクタ
  /// @param[in] token トークン
  /// @param[in] left, right オペランド
  AstBinOp(TokenType token,
	   AstExpr* left,
	   AstExpr* right);

  /// @brief デストラクタ
  virtual
  ~AstBinOp();


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

  // トークン
  TokenType mToken;

  // 左オペランド
  AstExpr* mLeft;

  // 右オペランド
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBINOP_H
