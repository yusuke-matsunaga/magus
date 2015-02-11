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
  /// @param[in] stmt_num 文のリストの要素数
  /// @param[in] stmt_list 文のリスト
  /// @param[in] loc ファイル位置
  AstBlockStmt(ymuint num,
	       AstStatement** stmt_list,
	       const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstBlockStmt();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 文の数
  ymuint mStmtNum;

  // 文のリスト(ポインタ配列)
  AstStatement** mStmtList;

};

END_NAMESPACE_YM_YMSL

#endif // ASTBLOCKSTMT_H
