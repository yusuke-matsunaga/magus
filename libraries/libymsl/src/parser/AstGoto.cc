
/// @file AstGoto.cc
/// @brief AstGoto の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstGoto.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstGoto
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstGoto::AstGoto(AstSymbol* label,
		 const FileRegion& loc) :
  AstStatement(loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstGoto::~AstGoto()
{
}

// @brief 種類を返す．
StmtType
AstGoto::stmt_type() const
{
  return kGoto;
}

// @brief ラベルを得る．
//
// kGoto のみ有効
const AstSymbol*
AstGoto::label() const
{
  return mLabel;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstGoto::phase1(YmslScope* parent_scope,
		YmslTypeMgr* type_mgr)
{
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstGoto::phase2(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstGoto::calc_size()
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
AstGoto::compile(YmslDriver& driver,
		 YmslCodeList& code_list,
		 Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstGoto::print(ostream& s,
	       ymuint indent) const
{
  print_indent(s, indent);
  s << "goto " << mLabel->str_val() << endl;
}
#endif

END_NAMESPACE_YM_YMSL
