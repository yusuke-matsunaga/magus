
/// @file IrFuncCall.cc
/// @brief IrFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncCall.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
// @param[in] arglist 引数リスト
IrFuncCall::IrFuncCall(IrExpr* func,
			   const vector<IrExpr*>& arglist) :
  mFunc(func)
{
  mArgNum = arglist.size();
  mArgList = new IrExpr*[mArgNum];
  for (ymuint i = 0; i < mArgNum; ++ i) {
    mArgList[i] = arglist[i];
  }
}

// @brief デストラクタ
IrFuncCall::~IrFuncCall()
{
  delete [] mArgList;
}

// @brief 型を返す．
ExprType
IrFuncCall::expr_type() const
{
  return kFuncCall;
}

// @brief 関数本体を返す．
//
// kFuncCall のみ有効
IrExpr*
IrFuncCall::func_expr() const
{
  return mFunc;
}

// @brief 関数の引数の数を得る．
//
// kFuncCall のみ有効
ymuint
IrFuncCall::arglist_num() const
{
  return mArgNum;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kFuncCall のみ有効
IrExpr*
IrFuncCall::arglist_elem(ymuint pos) const
{
  ASSERT_COND( pos < arglist_num() );
  return mArgList[pos];
}

END_NAMESPACE_YM_YMSL
