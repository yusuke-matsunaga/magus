#ifndef ASTEXPR_H
#define ASTEXPR_H

/// @file AstExpr.h
/// @brief AstExpr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstExpr AstExpr.h "AstExpr.h"
/// @brief 式を表すクラス
//////////////////////////////////////////////////////////////////////
class AstExpr :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstExpr(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstExpr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 次の要素を得る．
  AstExpr*
  next() const;

  /// @brief 前の要素をセットする．
  void
  set_prev(AstExpr* prev);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const = 0;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 次の要素
  AstExpr* mNext;

};

END_NAMESPACE_YM_YMSL


#endif // ASTEXPR_H
