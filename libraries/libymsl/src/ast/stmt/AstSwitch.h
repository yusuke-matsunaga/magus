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
  /// @param[in] case_num case 文の数
  /// @param[in] case_list case 文のリスト
  /// @param[in] loc ファイル位置
  AstSwitch(AstExpr* expr,
	    ymuint case_num,
	    AstCaseItem** case_list,
	    const FileRegion& loc);

  /// @brief デストラクタ
  virtual
  ~AstSwitch();


public:
  //////////////////////////////////////////////////////////////////////
  // 外部インターフェイス
  //////////////////////////////////////////////////////////////////////

  /// @brief 種類を返す．
  virtual
  Type
  stmt_type() const;

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


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 式
  AstExpr* mExpr;

  // case-item の数
  ymuint mNum;

  // case-item のリスト(ポインタ配列)
  AstCaseItem** mCaseItemList;

};

END_NAMESPACE_YM_YMSL


#endif // ASTSWITCH_H
