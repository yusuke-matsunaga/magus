
/// @file AstContinue.cc
/// @brief AstContinue の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstContinue.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstContinue
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] loc ファイル位置
AstContinue::AstContinue(const FileRegion& loc) :
  AstStatement(loc)
{
}

// @brief デストラクタ
AstContinue::~AstContinue()
{
}

// @brief 種類を返す．
StmtType
AstContinue::stmt_type() const
{
  return kContinue;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstContinue::phase1(YmslScope* parent_scope,
		    YmslTypeMgr* type_mgr)
{
}

// @brief 関数の参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstContinue::phase2(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstContinue::calc_size()
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
AstContinue::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstContinue::print(ostream& s,
		   ymuint indent) const
{
  print_indent(s, indent);
  s << "continue" << endl;
}
#endif

END_NAMESPACE_YM_YMSL
