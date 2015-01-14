
/// @file AstBreak.cc
/// @brief AstBreak の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstBreak.h"

#include "YmslCodeList.h"
#include "YmslScope.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstBreak
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstBreak::AstBreak(const FileRegion& loc) :
  AstStatement(loc)
{
}

// @brief デストラクタ
AstBreak::~AstBreak()
{
}

// @brief 関数の登録を行う．
// @param[in] parent_scope 親のスコープ
void
AstBreak::phase1(YmslScope* parent_scope)
{
}

// @brief スコープの生成と参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstBreak::phase2(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstBreak::calc_size()
{
  return 2;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstBreak::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstBreak::print(ostream& s,
		ymuint indent) const
{
  print_indent(s, indent);
  s << "break" << endl;
}

END_NAMESPACE_YM_YMSL
