#ifndef ASTSWITCH_H
#define ASTSWITCH_H

/// @file AstSwitch.h
/// @brief AstSwitch のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStatement.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
/// @class AstSwitch AstSwitch.h "AstSwitch.h"
/// @brief switch 文を表す Ast
//////////////////////////////////////////////////////////////////////
class AstSwitch :
  public AstStatement
{
public:

  /// @brief コンストラクタ
  /// @param[in] expr 条件式
  /// @param[in] case_list case 文のリスト
  /// @param[in] loc ファイル位置
  AstSwitch(AstExpr* expr,
	    const vector<AstCaseItem*>& case_list,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstSwitch();


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

  // 式
  AstExpr* mExpr;

  // case-item のリスト
  vector<AstCaseItem*> mCaseItemList;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSWITCH_H
