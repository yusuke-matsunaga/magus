
/// @file AstIteOp.cc
/// @brief AstIteOp の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstIteOp.h"

#include "YmslCodeList.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstIteOp
//////////////////////////////////////////////////////////////////////

// @breif コンストラクタ
// @param[in] opr1, opr2, opr3 オペランド
AstIteOp::AstIteOp(AstExpr* opr1,
		   AstExpr* opr2,
		   AstExpr* opr3) :
  AstExpr(FileRegion(opr1->file_region(), opr3->file_region()))
{
  mOpr[0] = opr1;
  mOpr[1] = opr2;
  mOpr[2] = opr3;
}

// @brief デストラクタ
AstIteOp::~AstIteOp()
{
}

// @brief 変数の参照を解決する．
void
AstIteOp::resolve_var(YmslScope* parent_scope)
{
  for (ymuint i = 0; i < 3; ++ i) {
    mOpr[i]->resolve_var(parent_scope);
  }
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstIteOp::type_analysis()
{
}

// @brief 式の型を返す．
const YmslType*
AstIteOp::type()
{
  return mType;
}

// @brief 命令コードのサイズを計算する．
ymuint
AstIteOp::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstIteOp::compile(YmslDriver& driver,
		  YmslCodeList& code_list,
		  Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstIteOp::print(ostream& s) const
{
  mOpr[0]->print(s);
  s << " ? ";
  mOpr[1]->print(s);
  s << " : ";
  mOpr[2]->print(s);
}

END_NAMESPACE_YM_YMSL
