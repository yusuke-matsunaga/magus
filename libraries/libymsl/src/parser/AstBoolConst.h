#ifndef ASTBOOLCONST_H
#define ASTBOOLCONST_H

/// @file AstBoolConst.h
/// @brief AstBoolConst のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstBoolConst AstBoolConst.h "AstBoolConst.h"
/// @brief true/false を表すクラス
//////////////////////////////////////////////////////////////////////
class AstBoolConst :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr_type 式の種類 (kTrue/kFalse)
  /// @param[in] loc ファイル位置
  AstBoolConst(ExprType expr_type,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstBoolConst();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  virtual
  void
  print(ostream& s) const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式の種類
  ExprType mExprType;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBOOLCONST_H
