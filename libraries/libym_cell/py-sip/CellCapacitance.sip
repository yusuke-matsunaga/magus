
/// @file CellCapacitance.sip
/// @brief CellCapacitance の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellCapacitance
{
%TypeHeaderCode
#include "cell/CellCapacitance.h"
using nsYm::CellCapacitance;
%End

public:

  /// @brief 空のコンストラクタ
  CellCapacitance();

  /// @brief double からの変換コンストラクタ
  CellCapacitance(double v);

  /// @brief 無限大の値を作る．
  static
  CellCapacitance
  infty();

  /// @brief 値を得る．
  double
  value() const;

};