#ifndef ASTINCR_H
#define ASTINCR_H

/// @file AstIncr.h
/// @brief AstIncr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstIncr AstIncr.h "AstIncr.h"
/// @brief 増加文を表すクラス
//////////////////////////////////////////////////////////////////////
class AstIncr :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 対象の式
  /// @param[in] loc ファイル位置
  AstIncr(AstExpr* expr,
	  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstIncr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

  /// @brief 左辺式を返す．
  ///
  /// kAssignment のみ有効
  virtual
  const AstExpr*
  lhs_expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // デーテメンバ
  //////////////////////////////////////////////////////////////////////

  // 式
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTINCR_H
