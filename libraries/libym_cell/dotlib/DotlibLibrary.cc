
/// @file libym_dotlib/DotlibLibrary.cc
/// @brief DotlibLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibLibrary.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibLibrary::DotlibLibrary()
{
  init();
}

// @brief デストラクタ
DotlibLibrary::~DotlibLibrary()
{
}

// @brief 内容を初期化する．
void
DotlibLibrary::init()
{
  DotlibAttrMap::init();

  mDelayModel = CellLibrary::kDelayGenericCmos;
  mBusNamingStyle = NULL;
  mComment = NULL;
  mDate = NULL;
  mRevision = NULL;
  mCapacitiveLoadUnit = NULL;
  mCurrentUnit = NULL;
  mLeakagePowerUnit = NULL;
  mPullingResistanceUnit = NULL;
  mTimeUnit = NULL;
  mVoltageUnit = NULL;

  mCellList.clear();
}

// @brief 名前を返す．
ShString
DotlibLibrary::name() const
{
  return mName;
}

// @brief "delay_model" を返す．
CellLibrary::tDelayModel
DotlibLibrary::delay_model() const
{
  return mDelayModel;
}

// @brief "bus_naming_style" を返す．
const DotlibNode*
DotlibLibrary::bus_naming_style() const
{
  return mBusNamingStyle;
}

// @brief "comment" を返す．
const DotlibNode*
DotlibLibrary::comment() const
{
  return mComment;
}

// @brief "date" を返す．
const DotlibNode*
DotlibLibrary::date() const
{
  return mDate;
}

// @brief "revision" を返す．
const DotlibNode*
DotlibLibrary::revision() const
{
  return mRevision;
}

// @brief "capacitive_load_unit"->top() を返す．
double
DotlibLibrary::capacitive_load_unit() const
{
  return mCapacitiveLoadUnit;
}

// @brief "capacitive_load_unit"->top()->next() を返す．
string
DotlibLibrary::capacitive_load_unit_str() const
{
  return mCapacitiveLoadUnitStr;
}

// @brief "current_unit" を返す．
const DotlibNode*
DotlibLibrary::current_unit() const
{
  return mCurrentUnit;
}

// @brief "leakage_power_unit" を返す．
const DotlibNode*
DotlibLibrary::leakage_power_unit() const
{
  return mLeakagePowerUnit;
}

// @brief "pulling_resistance_unit" を返す．
const DotlibNode*
DotlibLibrary::pulling_resistance_unit() const
{
  return mPullingResistanceUnit;
}

// @brief "time_unit" を返す．
const DotlibNode*
DotlibLibrary::time_unit() const
{
  return mTimeUnit;
}

// @brief "voltage_unit" を返す．
const DotlibNode*
DotlibLibrary::voltage_unit() const
{
  return mVoltageUnit;
}

// @brief セル定義のリストを返す．
const list<const DotlibNode*>&
DotlibLibrary::cell_list() const
{
  return mCellList;
}

END_NAMESPACE_YM_DOTLIB
