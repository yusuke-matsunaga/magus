#ifndef YM_DOTLIB_DOTLIBLIBRARY_H
#define YM_DOTLIB_DOTLIBLIBRARY_H

/// @file ym_dotlib/DotlibLibrary.h
/// @brief DotlibLibrary のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_dotlib/dotlib_nsdef.h"


BEGIN_NAMESPACE_YM_DOTLIB

//////////////////////////////////////////////////////////////////////
/// @class DotlibLibrary DotlibLibrary.h "ym_dotlib/DotlibLibrary.h"
/// @brief DotlibNode の木から取り出したライブラリの情報を表すクラス
//////////////////////////////////////////////////////////////////////
class DotlibLibrary
{
public:

  /// @brief コンストラクタ
  DotlibLibrary();

  /// @brief デストラクタ
  ~DotlibLibrary();


public:

  /// @brief 名前を返す．
  ShString
  name() const;

  /// @brief "bus_naming_style" を返す．
  ShString
  bus_naming_style() const;

  /// @brief "comment" を返す．
  ShString
  comment() const;

  /// @brief "date" を返す．
  ShString
  date() const;

  /// @brief "revision" を返す．
  ShString
  revision() const;

  /// @brief "current_unit" を返す．
  ShString
  current_unit() const;

  /// @brief "leakage_power_unit" を返す．
  ShString
  leakage_power_unit() const;

  /// @brief "time_unit" を返す．
  ShString
  time_unit() const;

  /// @brief "voltage_unit" を返す．
  ShString
  voltage_unit() const;

  /// @brief セル定義のリストを返す．
  const list<const DotlibNode*>&
  cell_list() const;

  /// @brief その他の属性
};

END_NAMESPACE_YM_DOTLIB

#endif // YM_DOTLIB_DOTLIBLIBRARY_H
