#ifndef ASTCASEITEM_H
#define ASTCASEITEM_H

/// @file AstCaseItem.h
/// @brief AstCaseItem のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstCaseItem AstCaseItem.h "AstCaseItem.h"
/// @brief case ブロックを表す Ast
//////////////////////////////////////////////////////////////////////
class AstCaseItem :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] label ラベル
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstCaseItem(AstExpr* label,
	      AstStatement* stmt,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstCaseItem();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief ラベルを返す．
  const AstExpr*
  label() const;

  /// @brief 本体の文を返す．
  const AstStatement*
  stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ラベル
  AstExpr* mLabel;

  // 本体の文
  AstStatement* mStmt;

};

END_NAMESPACE_YM_YMSL

#endif // ASTCASEITEM_H
