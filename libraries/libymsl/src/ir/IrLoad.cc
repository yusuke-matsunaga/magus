
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
// @param[in] addr アドレス
IrLoad::IrLoad(const Type* type,
	       IrHandle* addr) :
  IrNode(kOpLoad, type),
  mAddress(addr)
{
}

// @brief デストラクタ
IrLoad::~IrLoad()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrLoad::is_static() const
{
  return false;
}

// @brief ロード/ストア対象のアドレスを得る．
//
// kOpLoad, kOpStore, kOpInc, kOpDec のみ有効
IrHandle*
IrLoad::address() const
{
  return mAddress;
}

END_NAMESPACE_YM_YMSL
