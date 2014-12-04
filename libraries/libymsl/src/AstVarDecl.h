#ifndef ASTVARDECL_H
#define ASTVARDECL_H

/// @file AstVarDecl.h
/// @brief AstVarDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "DeclBase.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstVarDecl AstVarDecl.h "AstVarDecl.h"
/// @brief 変数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstVarDecl :
  public AstDeclBase
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] init_expr 初期化式
  /// @param[in] loc ファイル位置
  AstVarDecl(ShString name,
	     YmslAst* type,
	     AstExpr* init_expr,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstVarDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  Ast*
  type() const;

  /// @brief 初期化式を返す．
  ///
  /// NULL の場合もある．
  AstExpr*
  init_expr() const;

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

  // 名前
  ShString mName;

  // 変数の型
  Ast* mType;

  // 初期化式
  AstExpr* mInitExpr;

};

END_NAMESPACE_YM_YMSL


#endif // ASTVARDECL_H
