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
  /// @param[in] init_expr 初期化式
  /// @param[in] global グローバル変数の時 true にするフラグ
  /// @param[in] loc ファイル位置
  AstVarDecl(ShString name,
	     AstType* type,
	     AstExpr* init_expr,
	     bool global,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstVarDecl();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 名前を得る．
  ShString
  name() const;

  /// @brief インデックス番号を返す．
  ymuint
  index() const;

  /// @brief 型を得る．
  AstType*
  type() const;

  /// @brief 初期化式を返す．
  ///
  /// NULL の場合もある．
  AstExpr*
  init_expr() const;

  /// @brief グローバル変数の時 true を返す．
  bool
  global() const;


public:
  //////////////////////////////////////////////////////////////////////
  // AstStatement の仮想関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 関数の登録を行う．
  /// @param[in] parent_scope 親のスコープ
  virtual
  void
  phase1(YmslScope* parent_scope);

  /// @brief スコープの生成と参照解決を行う．
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
  AstExpr* mInitExpr;

  // グローバル変数の時 true にするフラグ
  bool mGlobal;

};

END_NAMESPACE_YM_YMSL

#endif // ASTVARDECL_H
