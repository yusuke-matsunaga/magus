
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
IrStringConst::IrStringConst(const char* val) :
  mVal(val)
{
}

// @brief デストラクタ
IrStringConst::~IrStringConst()
{
}

// @brief opcode を返す．
OpCode
IrStringConst::opcode() const
{
  return kOpStringConst;
}

// @brief 文字列を返す．
const char*
IrStringConst::string_val() const
{
  return mVal;
}

END_NAMESPACE_YM_YMSL
