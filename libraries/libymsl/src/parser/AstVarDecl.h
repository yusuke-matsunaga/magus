#ifndef ASTVARDECL_H
#define ASTVARDECL_H

/// @file AstVarDecl.h
/// @brief AstVarDecl のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"
#include "YmUtils/ShString.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstVarDecl AstVarDecl.h "AstVarDecl.h"
/// @brief 変数宣言を表すクラス
//////////////////////////////////////////////////////////////////////
class AstVarDecl :
  public AstStatement
{
  friend class YmslModule;

public:

  /// @brief コンストラクタ
  /// @param[in] name 変数名
  /// @param[in] type 型
  /// @param[in] expr 初期化式
  /// @param[in] loc ファイル位置
  ///
  /// expr は NULL の場合もある．
  AstVarDecl(ShString name,
	     AstType* type,
	     AstExpr* expr,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstVarDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief インデックス番号を返す．
  ymuint
  index() const;


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

  /// @brief 式を返す．
  ///
  /// kExprStmt, kReturn, kVarDecl のみ有効
  virtual
  const AstExpr*
  expr() const;

#if 0
  /// @brief 要素の生成と関数以外の参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  /// @param[in] type_mgr 型マネージャ
  virtual
  void
  phase1(YmslScope* parent_scope,
	 YmslTypeMgr* type_mgr);

  /// @brief 参照解決を行う．
  /// @param[in] parent_scope 親のスコープ
  virtual
  void
  phase2(YmslScope* parent_scope);

  /// @brief 命令コードのサイズを計算する．
  virtual
  ymuint
  calc_size();

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
	  Ymsl_INT& addr);

  /// @brief 内容を表示する．(デバッグ用)
  /// @param[in] s 出力ストリーム
  /// @param[in] indent インデントレベル
  virtual
  void
  print(ostream& s,
	ymuint indent = 0) const;
#endif

private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 名前
  ShString mName;

  // インデックス番号
  ymuint mIndex;

  // 変数の型
  AstType* mType;

  // 初期化式
  AstExpr* mExpr;

};

END_NAMESPACE_YM_YMSL

#endif // ASTVARDECL_H
