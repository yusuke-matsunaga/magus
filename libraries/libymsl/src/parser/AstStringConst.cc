
/// @file AstStringConst.cc
/// @brief AstStringConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstStringConst.h"

#include "YmslCodeList.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstStringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstStringConst::AstStringConst(const char* val,
			       const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstStringConst::~AstStringConst()
{
}

// @brief 種類を返す．
ExprType
AstStringConst::expr_type() const
{
  return kStringConst;
}

// @brief 文字列値を返す．
//
// kStringConst のみ有効
const char*
AstStringConst::string_val() const
{
  return mVal;
}

#if 0
// @brief 変数の参照を解決する．
void
AstStringConst::resolve_var(YmslScope* parent_scope)
{
  // 何もしない．
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstStringConst::type_analysis()
{
  return true;
}

// @brief 式の型を返す．
const YmslType*
AstStringConst::type()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstStringConst::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstStringConst::compile(YmslDriver& driver,
			YmslCodeList& code_list,
			Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstStringConst::print(ostream& s) const
{
  s << mVal;
}
#endif

END_NAMESPACE_YM_YMSL
