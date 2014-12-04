#ifndef ASTELIF_H
#define ASTELIF_H

/// @file AstElif.h
/// @brief AstElif のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstElif AstElif.h "AstElif.h"
/// @brief case ブロックを表す Ast
//////////////////////////////////////////////////////////////////////
class AstElif :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] cond 条件式
  /// @param[in] block 本体
  /// @param[in] loc ファイル位置
  AstElif(AstExpr* cond,
	  AstBlock* block,
	  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstElif();


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

#endif // ASTELIF_H
