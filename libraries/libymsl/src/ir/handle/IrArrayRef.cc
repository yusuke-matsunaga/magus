
/// @file IrArrayRef.cc
/// @brief IrArrayRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrArrayRef.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrArrayRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] offset オフセット
IrArrayRef::IrArrayRef(IrNode* base,
		       IrNode* offset) :
  mBase(base),
  mOffset(offset)
{
}

// @brief デストラクタ
IrArrayRef::~IrArrayRef()
{
}

// @brief 名前を返す．
ShString
IrArrayRef::name() const
{
  return ShString();
}

// @brief 種類を返す．
IrHandle::HandleType
IrArrayRef::handle_type() const
{
  return kArrayRef;
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrArrayRef::is_static() const
{
  return false;
}

// @brief 配列本体の式を返す．
//
// kOpArrayRef, kOpArrayStore のみ有効
IrNode*
IrArrayRef::array_expr() const
{
  return mBase;
}

// @brief 配列のインデックスを返す．
//
// kOpArrayRef, kOpArrayStore のみ有効
IrNode*
IrArrayRef::array_index() const
{
  return mOffset;
}

END_NAMESPACE_YM_YMSL
