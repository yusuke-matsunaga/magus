
/// @file CellResistance.sip
/// @brief CellResistance の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellResistance
{
%TypeHeaderCode
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

  /// @brief 加算
  CellResistance
  operator+(const CellResistance& src) const;

  /// @brief 減算
  CellResistance
  operator-(const CellResistance& src) const;

};

/// @brief 等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator==(const CellResistance& left,
	   const CellResistance& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator<(const CellResistance& left,
	  const CellResistance& right);
