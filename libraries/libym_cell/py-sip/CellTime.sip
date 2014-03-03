
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

  /// @brief 加算
  CellTime
  operator+(const CellTime& src) const;

  /// @brief 減算
  CellTime
  operator-(const CellTime& src) const;

};

/// @brief 等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator==(const CellTime& left,
	   const CellTime& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator<(const CellTime& left,
	  const CellTime& right);
