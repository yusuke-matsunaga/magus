#ifndef ASTDECR_H
#define ASTDECR_H

/// @file AstDecr.h
/// @brief AstDecr のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstDecr AstDecr.h "AstDecr.h"
/// @brief 減少文を表すクラス
//////////////////////////////////////////////////////////////////////
class AstDecr :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 対象の式
  /// @param[in] loc ファイル位置
  AstDecr(AstExpr* expr,
	  const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstDecr();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
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

#endif // ASTDECR_H
