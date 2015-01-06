#ifndef ASTCASEITEM_H
#define ASTCASEITEM_H

/// @file AstCaseItem.h
/// @brief AstCaseItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstCaseItem AstCaseItem.h "AstCaseItem.h"
/// @brief case ブロックを表す Ast
//////////////////////////////////////////////////////////////////////
class AstCaseItem :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] stmt_list 本体の文
  /// @param[in] loc ファイル位置
  AstCaseItem(AstExpr* label,
	      AstStmtList* stmt_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstCaseItem();


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

  // ラベル
  AstExpr* mLabel;

};

END_NAMESPACE_YM_YMSL

#endif // ASTCASEITEM_H
