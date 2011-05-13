
/// @file libym_dotlib/DotlibPin.cc
/// @brief DotlibPin の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibPin.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibPin
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibPin::DotlibPin()
{
  init();
}

// @brief デストラクタ
DotlibPin::~DotlibPin()
{
}

// @brief 内容を初期化する．
void
DotlibPin::init()
{
  DotlibAttrMap::init();

  mFunction = NULL;
  mInternalNode = NULL;
  mPinFuncType = NULL;

  mTimingList.clear();
}

// @brief 名前を返す．
ShString
DotlibPin::name() const
{
  return mName;
}

// @brief "direction" を返す．
DotlibPin::tDirection
DotlibPin::direction() const
{
  return mDirection;
}

// @brief "capacitance" を返す．
double
DotlibPin::capacitance() const
{
  return mCapacitance;
}

// @brief "rise_capacitance" を返す．
double
DotlibPin::rise_capacitance() const
{
  return mRiseCapacitance;
}

// @brief "fall_capacitance" を返す．
double
DotlibPin::fall_capacitance() const
{
  return mFallCapacitance;
}

// @brief "max_fanout" を返す．
double
DotlibPin::max_fanout() const
{
  return mMaxFanout;
}

// @brief "min_fanout" を返す．
double
DotlibPin::min_fanout() const
{
  return mMinFanout;
}

// @brief "max_capacitance" を返す．
double
DotlibPin::max_capacitance() const
{
  return mMaxCapacitance;
}

// @brief "min_capacitance" を返す．
double
DotlibPin::min_capacitance() const
{
  return mMinCapacitance;
}

// @brief "max_transition" を返す．
double
DotlibPin::max_transition() const
{
  return mMaxTransition;
}

// @brief "min_transition" を返す．
double
DotlibPin::min_transition() const
{
  return mMinTransition;
}

// @brief "function" を返す．
const DotlibNode*
DotlibPin::function() const
{
  return mFunction;
}

// @brief "internal_node" を返す．
const DotlibNode*
DotlibPin::internal_node() const
{
  return mInternalNode;
}

// @brief "pin_func_type" を返す．
const DotlibNode*
DotlibPin::pin_func_type() const
{
  return mPinFuncType;
}

// @brief "timing" グループのリストを得る．
const list<const DotlibNode*>&
DotlibPin::timing_list() const
{
  return mTimingList;
}

END_NAMESPACE_YM_DOTLIB
