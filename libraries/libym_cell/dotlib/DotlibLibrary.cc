
/// @file libym_dotlib/DotlibLibrary.cc
/// @brief DotlibLibrary の実装ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibLibrary.h"
#include "DotlibNode.h"
#include "DotlibAttr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
// クラス DotlibLibrary
//////////////////////////////////////////////////////////////////////

// @brief コンストラクタ
DotlibLibrary::DotlibLibrary()
{
}

// @brief デストラクタ
DotlibLibrary::~DotlibLibrary()
{
}

// @brief 内容を初期化する．
bool
DotlibLibrary::set_data(const DotlibNode* lib_node)
{
  init();

  mTechnology = CellLibrary::kTechCmos;
  mDelayModel = CellLibrary::kDelayGenericCmos;
  mBusNamingStyle = NULL;
  mComment = NULL;
  mDate = NULL;
  mRevision = NULL;
  mCapacitiveLoadUnit = 0.0;
  mCurrentUnit = NULL;
  mLeakagePowerUnit = NULL;
  mPullingResistanceUnit = NULL;
  mTimeUnit = NULL;
  mVoltageUnit = NULL;

  mLutTemplateList.clear();
  mCellList.clear();

  // ライブラリ名をを得る．
  mName = lib_node->group_value()->get_string_from_value_list();

  // 属性のリストを作る．
  for (const DotlibAttr* attr = lib_node->attr_top();
       attr; attr = attr->next()) {
    ShString attr_name = attr->attr_name();
    const DotlibNode* attr_value = attr->attr_value();
    if ( attr_name == "lu_table_template" ) {
      mLutTemplateList.push_back(attr_value);
    }
    else if ( attr_name == "cell" ) {
      mCellList.push_back(attr_value);
    }
    else {
      add(attr_name, attr_value);
    }
  }

  // 'technology' を取り出す．
  const DotlibNode* tech_node = NULL;
  if ( !get_singleton_or_null("technology", tech_node) ) {
    return false;
  }
  if ( tech_node ) {
    ShString str = tech_node->get_string_from_value_list();
    if ( str == "cmos" ) {
      mTechnology = CellLibrary::kTechCmos;
    }
    else if ( str == "fpga" ) {
      mTechnology = CellLibrary::kTechFpga;
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      tech_node->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error. only 'asic' or 'fpga' are allowed.");
      return false;
    }
  }

  // 'delay_model' を取り出す．
  const DotlibNode* dm_node = NULL;
  if ( !get_singleton_or_null("delay_model", dm_node) ) {
    return false;
  }
  CellLibrary::tDelayModel delay_model = CellLibrary::kDelayGenericCmos;
  if ( dm_node != NULL ) {
    ShString value = dm_node->string_value();
    if ( value == "generic_cmos" ) {
      delay_model = CellLibrary::kDelayGenericCmos;
    }
    else if ( value == "table_lookup" ) {
      delay_model = CellLibrary::kDelayTableLookup;
    }
    else if ( value == "piecewise_cmos" ) {
      delay_model = CellLibrary::kDelayPiecewiseCmos;
    }
    else if ( value == "cmos2" ) {
      delay_model = CellLibrary::kDelayCmos2;
    }
    else if ( value == "dcm" ) {
      delay_model = CellLibrary::kDelayDcm;
    }
    else {
      ostringstream buf;
      buf << value << ": Illegal value for 'delay_model'."
	  << " 'generic_cmos', 'table_lookup', "
	  << "'piecewise_cmos', 'cmos2' or 'dcm' are expected.";
      MsgMgr::put_msg(__FILE__, __LINE__,
		      dm_node->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      buf.str());
      return false;
    }
  }
  mDelayModel = delay_model;

  // 'bus_naming_style' を取り出す．
  if ( !get_singleton_or_null("bus_naming_style", mBusNamingStyle) ) {
    return false;
  }

  // 'comment' を取り出す．
  if ( !get_singleton_or_null("comment", mComment) ) {
    return false;
  }

  // 'date' を取り出す．
  if ( !get_singleton_or_null("date", mDate) ) {
    return false;
  }

  // 'revision' を取り出す．
  if ( !get_singleton_or_null("revision", mRevision) ) {
    return false;
  }

  // 'current_unit' を取り出す．
  if ( !get_singleton_or_null("current_unit", mCurrentUnit) ) {
    return false;
  }

  // 'leakage_power_unit' を取り出す．
  if ( !get_singleton_or_null("leakage_power_unit", mLeakagePowerUnit) ) {
    return false;
  }

  // 'pulling_resistance_unit' を取り出す．
  if ( !get_singleton_or_null("pulling_resistance_unit",
			      mPullingResistanceUnit) ) {
    return false;
  }

  // 'capacitive_load_unit' を取り出す．
  const DotlibNode* clu = NULL;
  if ( !get_singleton_or_null("capacitive_load_unit", clu) ) {
    return false;
  }
  if ( clu ) {
    // clu は ComplexHandler で読んでいるからリストのはず．
    assert_cond( clu->is_list(), __FILE__, __LINE__);
    if ( clu->list_size() != 2 ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      clu->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error");
      return false;
    }
    const DotlibNode* top = clu->list_elem(0);
    if ( top->is_int() ) {
      mCapacitiveLoadUnit = top->int_value();
    }
    else if ( top->is_float() ) {
      mCapacitiveLoadUnit = top->float_value();
    }
    else {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      top->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error, a number expected");
      return false;
    }
    const DotlibNode* next = clu->list_elem(1);
    if ( !next->is_string() ) {
      MsgMgr::put_msg(__FILE__, __LINE__,
		      next->loc(),
		      kMsgError,
		      "DOTLIB_PARSER",
		      "Syntax error, a string expected");
      return false;
    }
    mCapacitiveLoadUnitStr = next->string_value();
  }

  // 'time_unit' を取り出す．
  if ( !get_singleton_or_null("time_unit", mTimeUnit) ) {
    return false;
  }

  // 'voltage_unit' を取り出す．
  if ( !get_singleton_or_null("voltage_unit", mVoltageUnit) ) {
    return false;
  }

  return true;
}

// @brief 名前を返す．
ShString
DotlibLibrary::name() const
{
  return mName;
}

// @brief "technology" を返す．
CellLibrary::tTechnology
DotlibLibrary::technology() const
{
  return mTechnology;
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

// @brief lu_table_template のリストを返す．
const list<const DotlibNode*>&
DotlibLibrary::lut_template_list() const
{
  return mLutTemplateList;
}

// @brief セル定義のリストを返す．
const list<const DotlibNode*>&
DotlibLibrary::cell_list() const
{
  return mCellList;
}

END_NAMESPACE_YM_DOTLIB
