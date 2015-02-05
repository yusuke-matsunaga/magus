#ifndef ASTCONSTDECL_H
#define ASTCONSTDECL_H

/// @file AstConstDecl.h
/// @brief AstConstDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstConstDecl AstConstDecl.h "AstConstDecl.h"
/// @brief 定数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstConstDecl :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] name 定数名
  /// @param[in] type 型
  /// @param[in] expr 初期化式
  /// @param[in] loc ファイル位置
  AstConstDecl(AstSymbol* name,
	       AstType* type,
	       AstExpr* expr,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstConstDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // AstStatement の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

  /// @brief 名前を返す．
  ///
  /// kEnumDecl, kFuncDecl, kVarDecl, kConstDecl のみ有効
  virtual
  const AstSymbol*
  name() const;

  /// @brief 型を返す．
  ///
  /// kFuncDecl, kVarDecl, kConstDecl のみ有効
  virtual
  const AstType*
  type() const;

  /// @brief 式を返す．
  ///
  /// kExprStmt, kReturn, kVarDecl, kConstDecl のみ有効
  virtual
  const AstExpr*
  expr() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  AstSymbol* mName;

  // 定数の型
  AstType* mType;

  // 初期化式
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTCONSTDECL_H
