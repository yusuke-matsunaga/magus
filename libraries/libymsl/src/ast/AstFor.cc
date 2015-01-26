
/// @file AstFor.cc
/// @brief AstFor の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstFor.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFor
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] init 初期化文
// @param[in] cond 条件式
// @param[in] next 増加文
// @param[in] stmt 本体の文
// @param[in] loc ファイル位置
AstFor::AstFor(AstStatement* init,
	       AstExpr* cond,
	       AstStatement* next,
	       AstStatement* stmt,
	       const FileRegion& loc) :
  AstStatement(loc),
  mInit(init),
  mExpr(cond),
  mNext(next),
  mStmt(stmt)
{
}

// @brief デストラクタ
AstFor::~AstFor()
{
}

// @brief 種類を返す．
StmtType
AstFor::stmt_type() const
{
  return kFor;
}

// @brief 条件式を返す．
//
// kDoWhile, kFor, kIf, kWhile, kSwitch のみ有効
const AstExpr*
AstFor::expr() const
{
  return mExpr;
}

// @brief 初期化文を返す．
//
// kFor のみ有効
const AstStatement*
AstFor::init_stmt() const
{
  return mInit;
}

// @brief 増加文を返す．
//
// kFor のみ有効
const AstStatement*
AstFor::next_stmt() const
{
  return mNext;
}

// @brief 本体の文を返す．
//
// kFuncDecl, kFor, kDoWhile, kWhile, kIf のみ有効
const AstStatement*
AstFor::stmt() const
{
  return mStmt;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstFor::phase1(YmslScope* parent_scope,
	       YmslTypeMgr* type_mgr)
{
}

// @brief 関数の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstFor::phase2(YmslScope* parent_scope)
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
#endif

END_NAMESPACE_YM_YMSL
