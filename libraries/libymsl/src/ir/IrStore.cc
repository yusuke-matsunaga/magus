
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
// @param[in] base ベースアドレス
// @param[in] offset オフセット
// @param[in] val 値
IrStore::IrStore(IrNode* base,
		 IrNode* offset,
		 IrNode* val) :
  IrNode(kOpStore, NULL),
  mBase(base),
  mOffset(offset),
  mStoreVal(val)
{
}

// @brief デストラクタ
IrStore::~IrStore()
{
}

// @brief ベースアドレスを返す．
IrNode*
IrStore::base() const
{
  return mBase;
}

// @brief オフセットを返す．
IrNode*
IrStore::offset() const
{
  return mOffset;
}

// @brief 書き込む値を返す．
IrNode*
IrStore::store_val() const
{
  return mStoreVal;
}

END_NAMESPACE_YM_YMSL
