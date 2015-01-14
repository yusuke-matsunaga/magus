
/// @file AstFor.cc
/// @brief AstFor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstFor.h"

#include "YmslCodeList.h"
#include "YmslDriver.h"
#include "YmslLabel.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] stmt_list 本体の文
// @param[in] loc ファイル位置
AstFor::AstFor(AstStatement* init,
	       AstExpr* cond,
	       AstStatement* next,
	       AstStmtList* stmt_list,
	       const FileRegion& loc) :
  AstBlockStmt(stmt_list, loc),
  mInit(init),
  mCond(cond),
  mNext(next)
{
}

// @brief デストラクタ
AstFor::~AstFor()
{
}

// @brief スコープの生成と変数名の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstFor::phase1(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstFor::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstFor::compile(YmslDriver& driver,
		YmslCodeList& code_list,
		Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstFor::print(ostream& s,
	      ymuint indent) const
{
#if 0
  print_indent(s, indent);
  s << "for (";
  mInit->print(s);
  s << "; ";
  mCond->print(s);
  s << "; ";
  mNext->print(s);
  s << ")" << endl;

  block()->print(s, indent);
#endif
}

END_NAMESPACE_YM_YMSL
