#ifndef ASTFUNCCALL_H
#define ASTFUNCCALL_H

/// @file AstFuncCall.h
/// @brief AstFuncCall のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstExpr.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFuncCall AstFuncCall.h "AstFuncCall.h"
/// @brief 関数呼び出しを表す Ast
//////////////////////////////////////////////////////////////////////
class AstFuncCall :
  public AstExpr
{
public:

  /// @brief コンストラクタ
  /// @param[in] func 関数
  /// @param[in] expr_list 引数のリスト
  /// @param[in] loc ファイル位置
  AstFuncCall(AstFuncDecl* func,
	      AstExpr* expr_list,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFuncCall();


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

  // 関数
  AstFuncDecl* mFunc;

  // 引数のリスト
  vector<AstExpr*> mExprList;

};

END_NAMESPACE_YM_YMSL


#endif // ASTFUNCCALL_H
