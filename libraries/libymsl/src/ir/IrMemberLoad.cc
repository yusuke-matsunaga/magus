
/// @file IrMemberLoad.cc
/// @brief IrMemberLoad の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMemberLoad.h"
#include "Var.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMemberLoad
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] var メンバ変数
IrMemberLoad::IrMemberLoad(IrNode* base,
			   const Var* var) :
  IrNode(kOpMemberLoad, var->value_type()),
  mBase(base),
  mVar(var)
{
}

// @brief デストラクタ
IrMemberLoad::~IrMemberLoad()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrMemberLoad::is_static() const
{
  return false;
}

// @brief オブジェクトを指す式を返す．
//
// kOpMemberLoad, kOpMemberStore のみ有効
IrNode*
IrMemberLoad::obj_expr() const
{
  return mBase;
}

// @brief 変数を返す．
//
// kOpVarRef, kOpLoad, kOpStore, kOpMemberLoad, kOpMemberStore のみ有効
const Var*
IrMemberLoad::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL
