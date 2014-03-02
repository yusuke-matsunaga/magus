
/// @file CellArea.sip
/// @brief CellArea の sip ファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2014 Yusuke Matsunaga
/// All rights reserved.

class CellArea
{
%TypeHeaderCode
#include "cell/CellArea.h"
using nsYm::CellArea;
%End

public:

  /// @brief 空のコンストラクタ
  CellArea();

  /// @brief double からの変換コンストラクタ
  CellArea(double v);

  /// @brief 無限大の値を作る．
  static
  CellArea
  infty();

  /// @brief 値を得る．
  double
  value() const;

  /// @brief 加算
  CellArea
  operator+(const CellArea& src) const;

  /// @brief 減算
  CellArea
  operator-(const CellArea& src) const;

};

/// @brief 等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator==(const CellArea& left,
	   const CellArea& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellArea
bool
operator<(const CellArea& left,
	  const CellArea& right);
