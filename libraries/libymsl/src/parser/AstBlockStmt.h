#ifndef ASTBLOCKSTMT_H
#define ASTBLOCKSTMT_H

/// @file AstBlockStmt.h
/// @brief AstBlockStmt のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstBlockStmt AstBlockStmt.h "AstBlockStmt.h"
/// @brief block文を表す AstStatement
//////////////////////////////////////////////////////////////////////
class AstBlockStmt :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] stmt_list 本体の文
  /// @param[in] loc ファイル位置
  AstBlockStmt(AstStmtList* stmt_list,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstBlockStmt();


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

  // 本体
  vector<AstStatement*> mStmtList;

  // スコープ
  YmslScope* mScope;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBLOCKSTMT_H
