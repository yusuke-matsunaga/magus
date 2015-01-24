
/// @file AstFuncCall.cc
/// @brief AstFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.


#include "AstFuncCall.h"
#include "AstList.h"


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
AstFuncCall::func() const
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

END_NAMESPACE_YM_YMSL
