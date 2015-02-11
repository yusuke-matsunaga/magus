#ifndef ASTVARDECL_H
#define ASTVARDECL_H

/// @file AstVarDecl.h
/// @brief AstVarDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstVarDecl AstVarDecl.h "AstVarDecl.h"
/// @brief 変数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstVarDecl :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] expr 初期化式
  /// @param[in] loc ファイル位置
  ///
  /// expr は NULL の場合もある．
  AstVarDecl(AstSymbol* name,
	     AstType* type,
	     AstExpr* expr,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstVarDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // AstStatement の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

  /// @brief 名前を返す．
  ///
  /// kEnumDecl, kFuncDecl, kVarDecl のみ有効
  virtual
  const AstSymbol*
  name() const;

  /// @brief 型を返す．
  ///
  /// kFuncDecl, kVarDecl のみ有効
  virtual
  const AstType*
  type() const;

  /// @brief 式を返す．
  ///
  /// kExprStmt, kReturn, kVarDecl のみ有効
  virtual
  const AstExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  AstSymbol* mName;

  // 変数の型
  AstType* mType;

  // 初期化式
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTVARDECL_H
