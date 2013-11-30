#ifndef YM_DOTLIB_DOTLIBLIBRARY_H
#define YM_DOTLIB_DOTLIBLIBRARY_H

/// @file ym_dotlib/DotlibLibrary.h
/// @brief DotlibLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "DotlibAttrMap.h"
#include "ym_cell/CellLibrary.h"
#include "ym_utils/MsgMgr.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibLibrary DotlibLibrary.h "DotlibLibrary.h"
/// @brief DotlibNode の木から取り出したライブラリの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibLibrary :
  public DotlibAttrMap
{
public:

  /// @brief コンストラクタ
  DotlibLibrary();

  /// @brief デストラクタ
  ~DotlibLibrary();


public:

  /// @brief 内容を設定する．
  bool
  set_data(const DotlibNode* lib_node);

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief "technology" を返す．
  tCellTechnology
  technology() const;

  /// @brief "delay_model" を返す．
  tCellDelayModel
  delay_model() const;

  /// @brief "bus_naming_style" を返す．
  const DotlibNode*
  bus_naming_style() const;

  /// @brief "comment" を返す．
  const DotlibNode*
  comment() const;

  /// @brief "date" を返す．
  const DotlibNode*
  date() const;

  /// @brief "revision" を返す．
  const DotlibNode*
  revision() const;

  /// @brief "capacitive_load_unit"->top() の値を返す．
  /// @note 未定義なら 0.0 が返る．
  double
  capacitive_load_unit() const;

  /// @brief "capacitive_load_unit"->top()->next() の値を返す．
  string
  capacitive_load_unit_str() const;

  /// @brief "current_unit" を返す．
  const DotlibNode*
  current_unit() const;

  /// @brief "leakage_power_unit" を返す．
  const DotlibNode*
  leakage_power_unit() const;

  /// @brief "pulling_resistance_unit" を返す．
  const DotlibNode*
  pulling_resistance_unit() const;

  /// @brief "time_unit" を返す．
  const DotlibNode*
  time_unit() const;

  /// @brief "voltage_unit" を返す．
  const DotlibNode*
  voltage_unit() const;

  /// @brief lu_table_template のリストを返す．
  const list<const DotlibNode*>&
  lut_template_list() const;

  /// @brief セル定義のリストを返す．
  const list<const DotlibNode*>&
  cell_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ライブラリ名
  ShString mName;

  // "technology"
  tCellTechnology mTechnology;

  // "delay_model"
  tCellDelayModel mDelayModel;

  // "bus_naming_style"
  const DotlibNode* mBusNamingStyle;

  // "comment"
  const DotlibNode* mComment;

  // "date"
  const DotlibNode* mDate;

  // "revision"
  const DotlibNode* mRevision;

  // "capacitive_load_unit->top"
  double mCapacitiveLoadUnit;

  // "capacitive_load_unit->top->next"
  ShString mCapacitiveLoadUnitStr;

  // "current_unit"
  const DotlibNode* mCurrentUnit;

  // "leakage_power_unit"
  const DotlibNode* mLeakagePowerUnit;

  // "pulling_resistance_unit"
  const DotlibNode* mPullingResistanceUnit;

  // "time_unit"
  const DotlibNode* mTimeUnit;

  // "voltage_unit"
  const DotlibNode* mVoltageUnit;

  // lu_table_template のリスト
  list<const DotlibNode*> mLutTemplateList;

  // セル定義のリスト
  list<const DotlibNode*> mCellList;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBLIBRARY_H
