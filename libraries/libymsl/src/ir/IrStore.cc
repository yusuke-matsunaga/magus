
/// @file IrStore.cc
/// @brief IrStore の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrStore.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrStore
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] var 変数
// @param[in] val 値
IrStore::IrStore(const Var* var,
		 IrNode* val) :
  IrNode(kOpStore, NULL),
  mVar(var),
  mStoreVal(val)
{
}

// @brief デストラクタ
IrStore::~IrStore()
{
}

// @brief 変数を返す．
//
// kOpVarRef, kOpLoad, kOpStore のみ有効
const Var*
IrStore::var() const
{
  return mVar;
}

// @brief 書き込む値を返す．
IrNode*
IrStore::store_val() const
{
  return mStoreVal;
}

END_NAMESPACE_YM_YMSL
