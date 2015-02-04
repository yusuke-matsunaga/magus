
/// @file IrLoad.cc
/// @brief IrLoad の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrLoad.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrLoad
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] type 型
// @param[in] base ベースアドレス
// @param[in] offset オフセット
IrLoad::IrLoad(const Type* type,
	       IrNode* base,
	       IrNode* offset) :
  IrNode(kOpLoad, type),
  mBase(base),
  mOffset(offset)
{
}

// @brief デストラクタ
IrLoad::~IrLoad()
{
}

// @brief ベースアドレスを返す．
IrNode*
IrLoad::base() const
{
  return mBase;
}

// @brief オフセットを返す．
IrNode*
IrLoad::offset() const
{
  return mOffset;
}

END_NAMESPACE_YM_YMSL
