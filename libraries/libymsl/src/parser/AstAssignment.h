#ifndef ASTASSIGNMENT_H
#define ASTASSIGNMENT_H

/// @file AstAssignment.h
/// @brief AstAssignment のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstAssignment AstAssignment.h "AstAssignment.h"
/// @brief 代入文を表す AstStatement
//////////////////////////////////////////////////////////////////////
class AstAssignment :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt_type 文の種類
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  /// @param[in] loc ファイル位置
  AstAssignment(StmtType stmt_type,
		AstLeaf* left,
		AstExpr* right,
		const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  StmtType
  stmt_type() const;

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

  // 文の種類
  StmtType mStmtType;

  // 左辺式
  AstLeaf* mLeft;

  // 右辺式
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTASSIGNMENT_H
