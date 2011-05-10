#ifndef YM_DOTLIB_DOTLIBLIBRARY_H
#define YM_DOTLIB_DOTLIBLIBRARY_H

/// @file ym_dotlib/DotlibLibrary.h
/// @brief DotlibLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/DotlibAttrMap.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibLibrary DotlibLibrary.h "ym_dotlib/DotlibLibrary.h"
/// @brief DotlibNode の木から取り出したライブラリの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibLibrary :
  public DotlibAttrMap
{
  friend class DotlibNode;

public:

  /// @brief コンストラクタ
  DotlibLibrary();

  /// @brief デストラクタ
  ~DotlibLibrary();


public:

  /// @brief 内容を初期化する．
  void
  clear();

  /// @brief 名前を返す．
  ShString
  name() const;

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

  /// @brief "current_unit" を返す．
  const DotlibNode*
  current_unit() const;

  /// @brief "leakage_power_unit" を返す．
  const DotlibNode*
  leakage_power_unit() const;

  /// @brief "time_unit" を返す．
  const DotlibNode*
  time_unit() const;

  /// @brief "voltage_unit" を返す．
  const DotlibNode*
  voltage_unit() const;

  /// @brief セル定義のリストを返す．
  const list<const DotlibNode*>&
  cell_list() const;


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // ライブラリ名
  ShString mName;

  // "bus_naming_style"
  const DotlibNode* mBusNamingStyle;

  // "comment"
  const DotlibNode* mComment;

  // "date"
  const DotlibNode* mDate;

  // "revision"
  const DotlibNode* mRevision;

  // "current_unit"
  const DotlibNode* mCurrentUnit;

  // "leakage_power_unit"
  const DotlibNode* mLeakagePowerUnit;

  // "time_unit"
  const DotlibNode* mTimeUnit;

  // "voltage_unit"
  const DotlibNode* mVoltageUnit;

  // セル定義のリスト
  list<const DotlibNode*> mCellList;

};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBLIBRARY_H
