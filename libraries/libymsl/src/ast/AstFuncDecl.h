#ifndef ASTFUNCDECL_H
#define ASTFUNCDECL_H

/// @file AstFuncDecl.h
/// @brief AstFuncDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstFuncDecl AstFuncDecl.h "AstFuncDecl.h"
/// @brief 関数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstFuncDecl :
  public AstStatement
{
  friend class YmslModule;

public:

  /// @brief コンストラクタ
  /// @param[in] name 関数名
  /// @param[in] type 型
  /// @param[in] param_num パラメータの数
  /// @param[in] param_list パラメータリスト
  /// @param[in] stmt 本体の文
  /// @param[in] loc ファイル位置
  AstFuncDecl(AstSymbol* name,
	      AstType* type,
	      ymuint param_num,
	      AstParam** param_list,
	      AstStatement* stmt,
	      const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstFuncDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // AstStatement の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

  /// @brief 名前を得る．
  virtual
  const AstSymbol*
  name() const;

  /// @brief 出力の型を返す．
  virtual
  const AstType*
  type() const;

  /// @brief パラメータリストの要素数を返す．
  ///
  /// kFuncDecl のみ有効
  virtual
  ymuint
  param_num() const;

  /// @brief パラメータ名を返す．
  /// @param[in] pos 位置 ( 0 <= pos < param_num() )
  ///
  /// kFuncDecl のみ有効
  virtual
  const AstSymbol*
  param_name(ymuint pos) const;

  /// @brief パラメータの型を返す．
  /// @param[in] pos 位置 ( 0 <= pos < param_num() )
  ///
  /// kFuncDecl のみ有効
  virtual
  const AstType*
  param_type(ymuint pos) const;

  /// @brief パラメータの初期値を返す．
  /// @param[in] pos 位置 ( 0 <= pos < param_num() )
  ///
  /// kFuncDecl のみ有効
  virtual
  const AstExpr*
  param_expr(ymuint pos) const;

  /// @brief 本体の文を返す．
  ///
  /// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
  virtual
  const AstStatement*
  stmt() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  AstSymbol* mName;

  // 出力の型
  AstType* mType;

  // パラメータの数
  ymuint mParamNum;

  // パラメータリスト(ポインタ配列)
  AstParam** mParamList;

  // 本体の文
  AstStatement* mStmt;

};

END_NAMESPACE_YM_YMSL

#endif // ASTFUNCDECL_H
