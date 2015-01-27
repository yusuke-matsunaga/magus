
/// @file AstSwitch.cc
/// @brief AstSwitch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstSwitch.h"
#include "AstCaseItem.h"
#include "AstList.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstSwitch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 条件式
// @param[in] case_num case 文の数
// @param[in] case_list case リスト
// @param[in] loc ファイル位置
AstSwitch::AstSwitch(AstExpr* expr,
		     ymuint case_num,
		     AstCaseItem** case_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr),
  mNum(case_num),
  mCaseItemList(case_list)
{
}

// @brief デストラクタ
AstSwitch::~AstSwitch()
{
}

// @brief 種類を返す．
StmtType
AstSwitch::stmt_type() const
{
  return kSwitch;
}

// @brief switch 文の case 数を返す．
//
// kSwitch のみ有効
ymuint
AstSwitch::switch_num() const
{
  return mNum;
}

// @brief switch 文の case ラベルを返す．
// @param[in] pos 位置 ( 0 <= pos < switch_num() )
//
// kSwitch のみ有効
const AstExpr*
AstSwitch::case_label(ymuint pos) const
{
  ASSERT_COND( pos < switch_num() );
  return mCaseItemList[pos]->label();
}

// @brief switch 文の case ブロックを返す．
// @param[in] pos 位置 ( 0 <= pos < switch_num() )
//
// kSwitch のみ有効
const AstStatement*
AstSwitch::case_stmt(ymuint pos) const
{
  ASSERT_COND( pos < switch_num() );
  return mCaseItemList[pos]->stmt();
}


#if 0

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstSwitch::print(ostream& s,
		 ymuint indent) const
{
  print_indent(s, indent);
  s << "switch ";
  mExpr->print(s);
  s << "{" << endl;

  ymuint n = mCaseItemList.size();
  for (ymuint i = 0; i < n; ++ i) {
    AstCaseItem* caseitem = mCaseItemList[i];
    caseitem->print(s, indent + 1);
  }

  print_indent(s, indent);
  s << "}" << endl;
}
#endif


//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstCaseItem::AstCaseItem(AstExpr* label,
			 AstStatement* stmt,
			 const FileRegion& loc) :
  Ast(loc),
  mLabel(label),
  mStmt(stmt)
{
}

// @brief デストラクタ
AstCaseItem::~AstCaseItem()
{
}

// @brief ラベルを返す．
const AstExpr*
AstCaseItem::label() const
{
  return mLabel;
}

// @brief 本体の文を返す．
const AstStatement*
AstCaseItem::stmt() const
{
  return mStmt;
}

#if 0
// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstCaseItem::print(ostream& s,
		   ymuint indent) const
{
  print_indent(s, indent);
  if ( mLabel != NULL ) {
    s << "case ";
    mLabel->print(s);
  }
  else {
    s << "default ";
  }
  s << ": " << endl;

  AstBlockStmt::print(s, indent);

  s << endl;
}
#endif

END_NAMESPACE_YM_YMSL
