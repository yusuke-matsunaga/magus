
/// @file IrArrayLoad.cc
/// @brief IrArrayLoad の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrArrayLoad.h"
#include "Type.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrArrayLoad
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] base ベースアドレス
// @param[in] offset オフセット
IrArrayLoad::IrArrayLoad(IrNode* base,
			 IrNode* offset) :
  IrNode(kOpArrayLoad, base->type()->elem_type()),
  mBase(base),
  mOffset(offset)
{
}

// @brief デストラクタ
IrArrayLoad::~IrArrayLoad()
{
}

// @brief 配列本体の式を返す．
//
// kOpArrayLoad, kOpArrayStore のみ有効
IrNode*
IrArrayLoad::array_expr() const
{
  return mBase;
}

// @brief 配列のインデックスを返す．
//
// kOpArrayLoad, kOpArrayStore のみ有効
IrNode*
IrArrayLoad::array_index() const
{
  return mOffset;
}

END_NAMESPACE_YM_YMSL
