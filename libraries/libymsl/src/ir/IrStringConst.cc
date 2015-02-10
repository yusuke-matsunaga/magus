
/// @file IrStringConst.cc
/// @brief IrStringConst の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrStringConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrStringConst
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
// @param[in] val 値
// @param[in] type 型
IrStringConst::IrStringConst(const Type* type,
			     const char* val) :
  IrNode(kStringConst, type),
  mVal(val)
{
}

// @brief デストラクタ
IrStringConst::~IrStringConst()
{
}

// @brief 静的評価可能か調べる．
//
// 要するに定数式かどうかということ
bool
IrStringConst::is_static() const
{
  return true;
}

// @brief 文字列を返す．
const char*
IrStringConst::string_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
