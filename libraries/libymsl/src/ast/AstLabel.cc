
/// @file AstLabel.cc
/// @brief AstLabel の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

#include "AstLabel.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstLabel
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] label ラベル
// @param[in] loc ファイル位置
AstLabel::AstLabel(AstSymbol* label,
		   const FileRegion& loc) :
  AstStatement(loc),
  mLabel(label)
{
}

// @brief デストラクタ
AstLabel::~AstLabel()
{
}

// @brief 種類を返す．
StmtType
AstLabel::stmt_type() const
{
  return kLabel;
}

// @brief ラベルを得る．
//
// kGoto, kLabel のみ有効
const AstSymbol*
AstLabel::label() const
{
  return mLabel;
}

#if 0
// @brief 要素の生成と関数以外の参照解決を行う．
// @param[in] parent_scope 親のスコープ
// @param[in] type_mgr 型マネージャ
void
AstLabel::phase1(YmslScope* parent_scope,
		 YmslTypeMgr* type_mgr)
{
}

// @brief 参照解決を行う．
// @param[in] parent_scope 親のスコープ
void
AstLabel::phase2(YmslScope* parent_scope)
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstLabel::calc_size()
{
  return 0;
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstLabel::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
// @param[in] indent インデントレベル
void
AstLabel::print(ostream& s,
		ymuint indent) const
{
  print_indent(s, indent);
  s << mLabel->str_val() << ":" << endl;
}
#endif

END_NAMESPACE_YM_YMSL
