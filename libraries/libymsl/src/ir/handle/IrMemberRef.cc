
/// @file IrMemberRef.cc
/// @brief IrMemberRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMemberRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMemberRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] var メンバ変数のハンドル
IrMemberRef::IrMemberRef(IrNode* base,
			 IrHandle* var) :
  mBase(base),
  mMemberVar(var)
{
}

// @brief デストラクタ
IrMemberRef::~IrMemberRef()
{
}

// @brief 名前を返す．
ShString
IrMemberRef::name() const
{
  return ShString();
}

// @brief 種類を返す．
IrHandle::HandleType
IrMemberRef::handle_type() const
{
  return kMemberRef;
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

// @brief 変数を得る．
//
// kMemberRef のみ有効
IrHandle*
IrMemberRef::member_var() const
{
  return mMemberVar;
}

END_NAMESPACE_YM_YMSL
