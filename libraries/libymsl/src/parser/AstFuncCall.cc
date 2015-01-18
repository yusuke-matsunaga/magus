
/// @file AstFuncCall.cc
/// @brief AstFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncCall.h"
#include "AstList.h"

#include "YmslCodeList.h"
#include "YmslVM.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス AstFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
// @param[in] expr_list 引数リスト
// @param[in] loc ファイル位置
AstFuncCall::AstFuncCall(AstExpr* func,
			 AstExprList* expr_list,
			 const FileRegion& loc) :
  AstExpr(loc),
  mFunc(func),
  mExprList(expr_list->size())
{
  ymuint pos = 0;
  for (AstExprList::Iterator p = expr_list->begin();
       !p.is_end(); p.next()) {
    mExprList[pos] = *p;
    ++ pos;
  }
}

// @brief デストラクタ
AstFuncCall::~AstFuncCall()
{
}

// @brief 種類を返す．
ExprType
AstFuncCall::expr_type() const
{
  return kFuncCall;
}

// @brief 関数本体を返す．
const AstExpr*
AstFuncCall::func_body() const
{
  return mFunc;
}

// @brief 引数リストの要素数を返す．
//
// kFuncCall のみ有効
ymuint
AstFuncCall::arglist_num() const
{
  return mExprList.size();
}

// @brief 引数リストの要素を返す．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
const AstExpr*
AstFuncCall::arglist_elem(ymuint pos) const
{
  ASSERT_COND( pos < arglist_num() );
  return mExprList[pos];
}

#if 0
// @brief 変数の参照を解決する．
void
AstFuncCall::resolve_var(YmslScope* parent_scope)
{
  ymuint n = mExprList.size();
  for (ymuint i = 0; i < n; ++ i) {
    mExprList[i]->resolve_var(parent_scope);
  }
}

// @brief 式の型を解析する．
// @return 引数の方が間違っていたら false を返す．
//
// 結果としてキャスト演算が挿入される場合もある．
bool
AstFuncCall::type_analysis()
{
  // 嘘
  return true;
}

// @brief 式の型を返す．
const YmslType*
AstFuncCall::type()
{
  //  return mFunc->type();
}

// @brief 命令コードのサイズを計算する．
ymuint
AstFuncCall::calc_size()
{
}

// @brief 命令コードを生成する．
// @param[in] driver ドライバ
// @param[in] code_list 命令コードの格納先
// @param[inout] addr 命令コードの現在のアドレス
//
// addr の値は更新される．
void
AstFuncCall::compile(YmslDriver& driver,
		     YmslCodeList& code_list,
		     Ymsl_INT& addr)
{
}

// @brief 内容を表示する．(デバッグ用)
// @param[in] s 出力ストリーム
void
AstFuncCall::print(ostream& s) const
{
  mFuncName->print(s);
  s << "(";
  {
    ymuint n = mExprList.size();
    const char* comma = "";
    for (ymuint i = 0; i < n; ++ i) {
      AstExpr* expr = mExprList[i];
      s << comma;
      comma = ", ";
      expr->print(s);
    }
  }
}
#endif

END_NAMESPACE_YM_YMSL
