
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
// @param[in] addr アドレス
// @param[in] val 値
IrStore::IrStore(IrHandle* addr,
		 IrNode* val) :
  IrNode(kOpStore, NULL),
  mAddress(addr),
  mStoreVal(val)
{
}

// @brief デストラクタ
IrStore::~IrStore()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrStore::is_static() const
{
  return false;
}

// @brief ロード/ストア対象のアドレスを得る．
//
// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
IrHandle*
IrStore::address() const
{
  return mAddress;
}

// @brief 書き込む値を返す．
IrNode*
IrStore::store_val() const
{
  return mStoreVal;
}

END_NAMESPACE_YM_YMSL
