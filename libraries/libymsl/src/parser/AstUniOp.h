#ifndef ASTUNIOP_H
#define ASTUNIOP_H

/// @file AstUniOp.h
/// @brief AstUniOp のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstUniOp AstUniOp.h "AstUniOp.h"
/// @brief 単項演算子を表す Ast
//////////////////////////////////////////////////////////////////////
class AstUniOp :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] token トークン
  /// @param[in] opr オペランド
  /// @param[in] loc ファイル位置
  AstUniOp(TokenType token,
	   AstExpr* opr,
	   const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstUniOp();


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

  // オペランド
  AstExpr* mOperand;

};

END_NAMESPACE_YM_YMSL

#endif // ASTUNIOP_H
