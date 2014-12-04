#ifndef ASTIF_H
#define ASTIF_H

/// @file AstIf.h
/// @brief AstIf のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIf AstIf.h "AstIf.h"
/// @brief if 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstIf :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cond 条件式
  /// @param[in] then_block then ブロック
  /// @param[in] elif_list elif ブロックのリスト
  /// @param[in] else_block else ブロック
  /// @param[in] loc ファイル位置
  AstIf(AstExpr* cond,
	AstBlock* then_block,
	Ast* elif_list,
	AstBlock* else_block,
	const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIf();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

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

  // 条件
  AstExpr* mCond;

};

END_NAMESPACE_YM_YMSL


#endif // ASTIF_H
