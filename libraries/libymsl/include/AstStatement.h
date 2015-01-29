#ifndef ASTSTATEMENT_H
#define ASTSTATEMENT_H

/// @file AstStatement.h
/// @brief AstStatement のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstStatement AstStatement.h "AstStatement.h"
/// @brief ステートメントを表すクラス
//////////////////////////////////////////////////////////////////////
class AstStatement :
  public Ast
{
public:

  /// @brief コンストラクタ
  /// @param[in] loc ファイル位置
  AstStatement(const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstStatement();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const = 0;

  /// @brief 名前を返す．
  ///
  /// kEnumDecl, kFuncDecl, kVarDecl, kLabel のみ有効
  virtual
  const AstSymbol*
  name() const;

  /// @brief 型を返す．
  ///
  /// kFuncDecl, kVarDecl のみ有効
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

  /// @brief enum 定数の数を返す．
  ///
  /// kEnumDecl のみ有効
  virtual
  ymuint
  enum_num() const;

  /// @brief enum 定数名を返す．
  /// @param[in] pos 位置 ( 0 <= pos < enum_num() )
  ///
  /// kEnumDecl のみ有効
  virtual
  const AstSymbol*
  enum_const(ymuint pos) const;

  /// @brief enum 定数の規定値を返す．
  /// @param[in] pos 位置 ( 0 <= pos < enum_num() )
  ///
  /// kEnumDecl のみ有効
  virtual
  const AstExpr*
  enum_const_expr(ymuint pos) const;

  /// @brief 左辺式を返す．
  ///
  /// kAssignment のみ有効
  virtual
  const AstExpr*
  lhs_expr() const;

  /// @brief 式を返す．
  ///
  /// kAssignment,
  /// kDoWhile, kFor, kIf, kWhile, kSwitch
  /// kExprStmt, kReturn, kVarDecl のみ有効
  virtual
  const AstExpr*
  expr() const;

  /// @brief 文のリストの要素数を返す．
  ///
  /// AstBlockStmt のみ有効
  virtual
  ymuint
  stmtlist_num() const;

  /// @brief 文のリストの要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < stmt_num() )
  ///
  /// AstBlockStmt のみ有効
  virtual
  const AstStatement*
  stmtlist_elem(ymuint pos) const;

  /// @brief 本体の文を返す．
  ///
  /// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
  virtual
  const AstStatement*
  stmt() const;

  /// @brief else 節を得る．
  ///
  /// kIf のみ有効
  virtual
  const AstStatement*
  else_stmt() const;

  /// @brief 初期化文を返す．
  ///
  /// kFor のみ有効
  virtual
  const AstStatement*
  init_stmt() const;

  /// @brief 増加文を返す．
  ///
  /// kFor のみ有効
  virtual
  const AstStatement*
  next_stmt() const;

  /// @brief switch 文の case 数を返す．
  ///
  /// kSwitch のみ有効
  virtual
  ymuint
  switch_num() const;

  /// @brief switch 文の case ラベルを返す．
  /// @param[in] pos 位置 ( 0 <= pos < switch_num() )
  ///
  /// kSwitch のみ有効
  virtual
  const AstExpr*
  case_label(ymuint pos) const;

  /// @brief switch 文の case ブロックを返す．
  /// @param[in] pos 位置 ( 0 <= pos < switch_num() )
  ///
  /// kSwitch のみ有効
  virtual
  const AstStatement*
  case_stmt(ymuint pos) const;

  /// @brief ラベルを得る．
  ///
  /// kGoto, kLabel のみ有効
  virtual
  const AstSymbol*
  label() const;

  /// @brief インポートするモジュール数を返す．
  ///
  /// kImport のみ有効
  virtual
  ymuint
  import_num() const;

  /// @brief インポートするモジュール名を返す．
  /// @param[in] pos 位置 ( 0 <= pos < inport_num() )
  ///
  /// kImport のみ有効
  virtual
  const AstSymbol*
  import_module(ymuint pos) const;

  /// @brief インポートするモジュールのエイリアスを返す．
  /// @param[in] pos 位置 ( 0 <= pos < inport_num() )
  ///
  /// kImport のみ有効
  virtual
  const AstSymbol*
  import_alias(ymuint pos) const;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSTATEMENT_H
