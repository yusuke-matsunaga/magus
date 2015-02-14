
/// @file IrMgr_const.cc
/// @brief IrMgr の実装ファイル(ConstVal生成関係)
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2015 Yusuke Matsunaga
/// All rights reserved.


#include "IrMgr.h"

#include "const_val/BooleanConst.h"
#include "const_val/IntConst.h"
#include "const_val/FloatConst.h"
#include "const_val/StringConst.h"


BEGIN_NAMESPACE_YM_YMSL

//////////////////////////////////////////////////////////////////////
// クラス IrMgr
//////////////////////////////////////////////////////////////////////

// @brief true 定数を生成する．
const ConstVal*
IrMgr::new_True()
{
  void* p = mAlloc.get_memory(sizeof(BooleanConst));
  return new (p) BooleanConst(mTypeMgr.boolean_type(), true);
}

// @brief False 定数を生成する．
const ConstVal*
IrMgr::new_False()
{
  void* p = mAlloc.get_memory(sizeof(BooleanConst));
  return new (p) BooleanConst(mTypeMgr.boolean_type(), false);
}

// @brief 整数値定数を生成する．
// @param[in] val 値
const ConstVal*
IrMgr::new_IntConst(Ymsl_INT val)
{
  void* p = mAlloc.get_memory(sizeof(IntConst));
  return new (p) IntConst(mTypeMgr.int_type(), val);
}

// @brief 実数値定数を生成する．
// @param[in] val 値
const ConstVal*
IrMgr::new_FloatConst(Ymsl_FLOAT val)
{
  void* p = mAlloc.get_memory(sizeof(FloatConst));
  return new (p) FloatConst(mTypeMgr.float_type(), val);
}

// @brief 文字列定数を生成する．
// @param[in] val 値
const ConstVal*
IrMgr::new_StringConst(const char* val)
{
  void* p = mAlloc.get_memory(sizeof(StringConst));
  return new (p) StringConst(mTypeMgr.string_type(), val);
}

END_NAMESPACE_YM_YMSL
