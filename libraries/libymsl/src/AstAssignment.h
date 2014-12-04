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
  /// @param[in] left 左辺
  /// @param[in] right 右辺
  AstAssignment(AstExpr* left,
		AstExpr* right);

  /// @brief デストラクタ
  virtual
  ~AstAssignment();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 型を得る．
  virtual
  AstType
  type() const;

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

  // 左辺式
  AstExpr* mLeft;

  // 右辺式
  AstExpr* mRight;

};

END_NAMESPACE_YM_YMSL

#endif // ASTASSIGNMENT_H
