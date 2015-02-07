
/// @file IrArrayStore.cc
/// @brief IrArrayStore の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrArrayStore.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrArrayStore
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] offset オフセット
// @param[in] val 値
IrArrayStore::IrArrayStore(IrNode* base,
			   IrNode* offset,
			   IrNode* val) :
  IrNode(kOpArrayStore, NULL),
  mBase(base),
  mOffset(offset),
  mStoreVal(val)
{
}

// @brief デストラクタ
IrArrayStore::~IrArrayStore()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrArrayStore::is_static() const
{
  return false;
}

// @brief 配列本体の式を返す．
//
// kOpArrayLoad, kOpArrayStore のみ有効
IrNode*
IrArrayStore::array_expr() const
{
  return mBase;
}

// @brief 配列のインデックスを返す．
//
// kOpArrayLoad, kOpArrayStore のみ有効
IrNode*
IrArrayStore::array_index() const
{
  return mOffset;
}

// @brief 書き込む値を返す．
IrNode*
IrArrayStore::store_val() const
{
  return mStoreVal;
}

END_NAMESPACE_YM_YMSL
