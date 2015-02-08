
/// @file IrArrayRef.cc
/// @brief IrArrayRef の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrArrayRef.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrArrayRef
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] offset オフセット
IrArrayRef::IrArrayRef(IrNode* base,
		       IrNode* offset) :
  IrNode(kOpArrayRef, base->type()->elem_type()),
  mBase(base),
  mOffset(offset)
{
}

// @brief デストラクタ
IrArrayRef::~IrArrayRef()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrArrayRef::is_static() const
{
  // 配列が不変ならロード結果も不変
  // だけど今は考えない．
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
