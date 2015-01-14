#ifndef ASTDOWHILE_H
#define ASTDOWHILE_H

/// @file AstDoWhile.h
/// @brief AstDoWhile のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstBlockStmt.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstDoWhile AstDoWhile.h "AstDoWhile.h"
/// @brief for 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstDoWhile :
  public AstBlockStmt
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt_list 本体の文
  /// @param[in] cond 条件式
  /// @param[in] loc ファイル位置
  AstDoWhile(AstStmtList* stmt_list,
	     AstExpr* cond,
	     const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstDoWhile();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
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

  // 条件式
  AstExpr* mCond;

};

END_NAMESPACE_YM_YMSL


#endif // ASTDOWHILE_H
