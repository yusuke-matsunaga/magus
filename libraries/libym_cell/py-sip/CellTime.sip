
/// @file CellTime.sip
/// @brief CellTime の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellTime
{
%TypeHeaderCode
#include "cell/CellTime.h"
using nsYm::CellTime;
%End

public:

  /// @brief 空のコンストラクタ
  CellTime();

  /// @brief double からの変換用コンストラクタ
  CellTime(double v);

  /// @brief 無限大の値を作る．
  static
  CellTime
  infty();

  /// @brief 値を得る．
  double
  value() const;

};