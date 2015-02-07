
/// @file IrFuncCall.cc
/// @brief IrFuncCall の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrFuncCall.h"
#include "Function.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrFuncCall
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] func 関数
// @param[in] arglist 引数リスト
IrFuncCall::IrFuncCall(const Function* func,
		       const vector<IrNode*>& arglist) :
  IrNode(kOpFuncCall, func->type()->function_output_type()),
  mFunc(func)
{
  mArgNum = arglist.size();
  mArgList = new IrNode*[mArgNum];
  for (ymuint i = 0; i < mArgNum; ++ i) {
    mArgList[i] = arglist[i];
  }
}

// @brief デストラクタ
IrFuncCall::~IrFuncCall()
{
  delete [] mArgList;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrFuncCall::is_static() const
{
  // 副作用を考えないと判断できない．
  // ので今はパス
  return false;
}

// @brief 関数本体を返す．
//
// kOpFuncCall のみ有効
const Function*
IrFuncCall::function() const
{
  return mFunc;
}

// @brief 関数の引数の数を得る．
//
// kOpFuncCall のみ有効
ymuint
IrFuncCall::arglist_num() const
{
  return mArgNum;
}

// @brief 関数の引数を得る．
// @param[in] pos 位置 ( 0 <= pos < arglist_num() )
//
// kOpFuncCall のみ有効
IrNode*
IrFuncCall::arglist_elem(ymuint pos) const
{
  ASSERT_COND( pos < arglist_num() );
  return mArgList[pos];
}

END_NAMESPACE_YM_YMSL
