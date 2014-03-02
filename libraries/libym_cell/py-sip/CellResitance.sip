
/// @file CellResistance.sip
/// @brief CellResistance の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellResitance
{
%TypeHeaderCoder
#include "cell/CellResistance.h"
using nsYm::CellResistance;
%End

public:

  /// @brief 空のコンストラクタ
  CellResistance();

  /// @brief double からの変換コンストラクタ
  CellResistance(double v);

  /// @brief 無限大の値を作る．
  static
  CellResistance
  infty();

  /// @brief 値を得る．
  double
  value() const;

};