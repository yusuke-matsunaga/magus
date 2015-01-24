
/// @file YmslFuncCall.cc
/// @brief YmslFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "YmslFuncCall.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス YmslFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
// @param[in] arglist 引数リスト
YmslFuncCall::YmslFuncCall(YmslLeaf* func,
			   const vector<YmslExpr*>& arglist) :
  mFunc(func)
{
  mArgNum = arglist.size();
  mArgList = new YmslExpr*[mArgNum];
  for (ymuint i = 0; i < mArgNum; ++ i) {
    mArgList[i] = arglist[i];
  }
}

// @brief デストラクタ
YmslFuncCall::~YmslFuncCall()
{
  delete [] mArgList;
}

// @brief 型を返す．
ExprType
YmslFuncCall::expr_type() const
{
  return kFuncCall;
}

// @brief 本体を返す．
//
// kMemberRef, kArrayRef, kFuncCall のみ有効
YmslLeaf*
YmslFuncCall::body() const
{
  return mFunc;
}

// @brief 関数の引数の数を得る．
//
// kFuncCall のみ有効
ymuint
YmslFuncCall::arglist_num() const
{
  return mArgNum;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
YmslExpr*
YmslFuncCall::arglist_elem(ymuint pos) const
{
  ASSERT_COND( pos < arglist_num() );
  return mArgList[pos];
}

END_NAMESPACE_YM_YMSL
