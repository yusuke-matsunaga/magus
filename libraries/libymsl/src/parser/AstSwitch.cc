
/// @file AstSwitch.cc
/// @brief AstSwitch の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstSwitch.h"
#include "AstCaseItem.h"
#include "AstExpr.h"
#include "AstList.h"

#include "YmslCodeList.h"
#include "YmslDriver.h"
#include "YmslLabel.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstSwitch
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 条件式
// @param[in] case_list case リスト
// @param[in] loc ファイル位置
AstSwitch::AstSwitch(AstExpr* expr,
		     AstCaseList* case_list,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr),
  mCaseItemList(case_list->size())
{
  ymuint pos = 0;
  for (AstCaseList::Iterator p = case_list->begin();
       !p.is_end(); p.next()) {
    mCaseItemList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstSwitch::~AstSwitch()
{
}

// @brief スコープの生成と変数名の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstSwitch::phase1(YmslScope* parent_scope)
{
  mExpr->resolve_var(parent_scope);
  ymuint n = mCaseItemList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mCaseItemList[i]->phase1(parent_scope);
  }
}

// @brief 命令コードのサイズを計算する．
ymuint
AstSwitch::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstSwitch::compile(YmslDriver& driver,
		   YmslCodeList& code_list,
		   Ymsl_INT& addr)
{
}

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


//////////////////////////////////////////////////////////////////////
// クラス AstCaseItem
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstCaseItem::AstCaseItem(AstExpr* label,
			 AstStmtList* stmt_list,
			 const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstCaseItem::~AstCaseItem()
{
}

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

END_NAMESPACE_YM_YMSL
