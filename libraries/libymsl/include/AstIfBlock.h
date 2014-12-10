#ifndef ASTIFBLOCK_H
#define ASTIFBLOCK_H

/// @file AstIfBlock
/// @brief AstIfBlock のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIfBlock AstIfBlock.h "AstIfBlock.h"
/// @brief if ブロックを表す Ast
//////////////////////////////////////////////////////////////////////
class AstIfBlock :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cond 条件式
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstIfBlock(AstExpr* cond,
	     AstBlock* block,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIfBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  AstIfBlock*
  next() const;

  /// @brief 前の要素をセットする．
  void
  set_prev(AstIfBlock* prev);

  /// @brief 条件を返す．
  AstExpr*
  cond() const;

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

  // 次の要素
  AstIfBlock* mNext;

};

END_NAMESPACE_YM_YMSL

#endif // ASTELIF_H
