#ifndef ASTDOWHILE_H
#define ASTDOWHILE_H

/// @file AstDoWhile.h
/// @brief AstDoWhile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstDoWhile AstDoWhile.h "AstDoWhile.h"
/// @brief for 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstDoWhile :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] block 本体
  /// @param[in] cond 条件式
  /// @param[in] loc ファイル位置
  AstDoWhile(AstBlock* block,
	     AstExpr* cond,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstDoWhile();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件式
  AstExpr* mCond;

};

END_NAMESPACE_YM_YMSL


#endif // ASTDOWHILE_H
