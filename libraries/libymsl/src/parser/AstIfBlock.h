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
  /// @param[in] stmt_list 本体の文
  /// @param[in] loc ファイル位置
  AstIfBlock(AstExpr* cond,
	     AstStmtList* stmt_list,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIfBlock();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 条件を返す．
  AstExpr*
  cond() const;

  /// @brief スコープの生成と変数名の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  virtual
  void
  phase1(YmslScope* parent_scope);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 条件
  AstExpr* mCond;

};

END_NAMESPACE_YM_YMSL

#endif // ASTELIF_H
