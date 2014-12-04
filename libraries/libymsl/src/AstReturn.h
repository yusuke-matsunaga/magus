#ifndef ASTRETURN_H
#define ASTRETURN_H

/// @file AstReturn.h
/// @brief AstReturn のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstReturn AstReturn.h "AstReturn.h"
/// @brief return 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstReturn :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 値
  /// @param[in] loc ファイル位置
  AstReturn(AstExpr* expr,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstReturn();


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

  // 値
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTRETURN_H
