
/// @file IrMemberStore.cc
/// @brief IrMemberStore の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMemberStore.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMemberStore
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] var メンバ変数
// @param[in] val 値
IrMemberStore::IrMemberStore(IrNode* base,
			     const Var* var,
			     IrNode* val) :
  IrNode(kOpMemberStore, NULL),
  mBase(base),
  mVar(var),
  mStoreVal(val)
{
}

// @brief デストラクタ
IrMemberStore::~IrMemberStore()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrMemberStore::is_static() const
{
  return false;
}

// @brief オブジェクトを指す式を返す．
//
// kOpMemberLoad, kOpMemberStore のみ有効
IrNode*
IrMemberStore::obj_expr() const
{
  return mBase;
}

// @brief 変数を返す．
//
// kOpVarRef, kOpLoad, kOpStore, kOpMemberLoad, kOpMemberStore のみ有効
const Var*
IrMemberStore::var() const
{
  return mVar;
}

// @brief 書き込む値を返す．
IrNode*
IrMemberStore::store_val() const
{
  return mStoreVal;
}

END_NAMESPACE_YM_YMSL
