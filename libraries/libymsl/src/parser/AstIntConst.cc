
/// @file AstIntCont.cc
/// @brief AstIntConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstIntConst.h"

#include "YmslCodeList.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIntConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstIntConst::AstIntConst(int val,
			 const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstIntConst::~AstIntConst()
{
}

// @brief 変数の参照を解決する．
void
AstIntConst::resolve_var(YmslScope* parent_scope)
{
  // 何もしない．
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstIntConst::type_analysis()
{
  return false;
}

// @brief 式の型を返す．
const YmslType*
AstIntConst::type()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstIntConst::calc_size()
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
AstIntConst::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
{
  code_list.write_opcode(addr, YMVM_PUSH_INT_IMM);
  code_list.write_opcode(addr, mVal);
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstIntConst::print(ostream& s) const
{
  s << mVal;
}

END_NAMESPACE_YM_YMSL
