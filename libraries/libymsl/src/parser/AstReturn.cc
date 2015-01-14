
/// @file AstReturn.cc
/// @brief AstReturn の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstReturn.h"
#include "AstExpr.h"

#include "YmslCodeList.h"
#include "YmslDriver.h"
#include "YmslLabel.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstReturn
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] expr 値
// @param[in] loc ファイル位置
AstReturn::AstReturn(AstExpr* expr,
		     const FileRegion& loc) :
  AstStatement(loc),
  mExpr(expr)
{
}

// @brief デストラクタ
AstReturn::~AstReturn()
{
}

// @brief スコープの生成と変数名の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstReturn::phase1(YmslScope* parent_scope)
{
  if ( mExpr != NULL ) {
    mExpr->resolve_var(parent_scope);
  }
}

// @brief 命令コードのサイズを計算する．
ymuint
AstReturn::calc_size()
{
  ymuint size = 1;
  if ( mExpr != NULL ) {
    size += mExpr->calc_size();
  }
  return size;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstReturn::compile(YmslDriver& driver,
		   YmslCodeList& code_list,
		   Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstReturn::print(ostream& s,
		 ymuint indent) const
{
  print_indent(s, indent);
  s << "return ";
  if ( mExpr != NULL ) {
    mExpr->print(s);
  }
  s << endl;
}

END_NAMESPACE_YM_YMSL
