
/// @file AstFloatConst.cc
/// @brief AstFloatConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFloatConst.h"

#include "YmslCodeList.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFloatConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] loc ファイル位置
AstFloatConst::AstFloatConst(double val,
			     const FileRegion& loc) :
  AstExpr(loc),
  mVal(val)
{
}

// @brief デストラクタ
AstFloatConst::~AstFloatConst()
{
}

// @brief 種類を返す．
ExprType
AstFloatConst::expr_type() const
{
  return kFloatConst;
}

// @brief 浮動小数点値を返す．
//
// kFloatConst のみ有効
double
AstFloatConst::float_val() const
{
  return mVal;
}

#if 0
// @brief 変数の参照を解決する．
void
AstFloatConst::resolve_var(YmslScope* parent_scope)
{
  // 何もしない．
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstFloatConst::type_analysis()
{
  return true;
}

// @brief 式の型を返す．
const YmslType*
AstFloatConst::type()
{
}

// @brief 命令コードのサイズを計算する．
ymuint
AstFloatConst::calc_size()
{
  // 値によって異なる．
  if ( mVal == 0.0 || mVal == 1.0 ) {
    return 1;
  }
  else {
    return 1 + (sizeof(Ymsl_FLOAT) / sizeof(Ymsl_CODE));
  }
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstFloatConst::compile(YmslDriver& driver,
		       YmslCodeList& code_list,
		       Ymsl_INT& addr)
{
  if ( mVal == 0.0 ) {
    code_list.write_opcode(addr, YMVM_PUSH_FLOAT_ZERO);
  }
  else if ( mVal == 1.0 ) {
    code_list.write_opcode(addr, YMVM_PUSH_FLOAT_ONE);
  }
  else {
    code_list.write_opcode(addr, YMVM_PUSH_FLOAT_IMM);
    code_list.write_float(addr, mVal);
  }
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstFloatConst::print(ostream& s) const
{
  s << mVal;
}
#endif

END_NAMESPACE_YM_YMSL
