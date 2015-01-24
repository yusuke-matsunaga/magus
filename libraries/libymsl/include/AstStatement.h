#ifndef ASTSTATEMENT_H
#define ASTSTATEMENT_H

/// @file AstStatement.h
/// @brief AstStatement のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "Ast.h"
#include "YmUtils/ShString.h"


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
  /// kEnumDecl, kFuncDecl, kVarDecl のみ有効
  virtual
  ShString
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

  /// @brief パラメータリストの要素を返す．
  /// @param[in] pos 位置 ( 0 <= pos < param_num() )
  ///
  /// kFuncDecl のみ有効
  virtual
  const AstParam*
  param(ymuint pos) const;

  /// @brief enum 定数の数を返す．
  ///
  /// kEnumDecl のみ有効
  virtual
  ymuint
  enum_num() const;

  /// @brief enum 定数を返す．
  /// @param[in] pos 位置 ( 0 <= pos < enum_num() )
  ///
  /// kEnumDecl のみ有効
  virtual
  const AstEnumConst*
  enum_const(ymuint pos) const;

  /// @brief 左辺式を返す．
  ///
  /// kAssignment のみ有効
  virtual
  const AstLeaf*
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

  /// @brief ラベルを得る．
  ///
  /// kGoto, kLabel のみ有効
  virtual
  const AstSymbol*
  label() const;

#if 0
  /// @brief 要素の生成と関数以外の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  /// @param[in] type_mgr 型マネージャ
  virtual
  void
  phase1(YmslScope* parent_scope,
	 YmslTypeMgr* type_mgr) = 0;

  /// @brief 関数の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  virtual
  void
  phase2(YmslScope* parent_scope) = 0;

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size() = 0;

  /// @brief 命令コードを生成する．
  /// @param[in] driver ドライバ
  /// @param[in] code_list 命令コードの格納先
  /// @param[inout] addr 命令コードの現在のアドレス
  ///
  /// addr の値は更新される．
  virtual
  void
  compile(YmslDriver& driver,
	  YmslCodeList& code_list,
	  Ymsl_INT& addr) = 0;

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const = 0;
#endif

#if 0
protected:
  //////////////////////////////////////////////////////////////////////
  // 内部で用いられる関数
  //////////////////////////////////////////////////////////////////////

  /// @brief インデントする．
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデント量
  static
  void
  print_indent(ostream& s,
	       ymuint indent);
#endif

};

END_NAMESPACE_YM_YMSL


#endif // ASTSTATEMENT_H
