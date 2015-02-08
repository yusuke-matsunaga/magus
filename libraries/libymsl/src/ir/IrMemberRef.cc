
/// @file IrMemberRef.cc
/// @brief IrMemberRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMemberRef.h"
#include "Var.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMemberRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] var メンバ変数
IrMemberRef::IrMemberRef(IrNode* base,
			 const Var* var) :
  IrNode(kOpMemberRef, var->value_type()),
  mBase(base),
  mVar(var)
{
}

// @brief デストラクタ
IrMemberRef::~IrMemberRef()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrMemberRef::is_static() const
{
  return false;
}

// @brief オブジェクトを指す式を返す．
//
// kOpMemberRef, kOpMemberStore のみ有効
IrNode*
IrMemberRef::obj_expr() const
{
  return mBase;
}

// @brief 変数を返す．
//
// kOpVarRef, kOpMemberRef のみ有効
const Var*
IrMemberRef::var() const
{
  return mVar;
}

END_NAMESPACE_YM_YMSL
