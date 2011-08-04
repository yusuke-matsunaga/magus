#ifndef YM_CELL_CELLCAPACITANCE_H
#define YM_CELL_CELLCAPACITANCE_H

/// @file　ym_cell/CellCapacitance.h
/// @brief CellCapacitance のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellCapacitance CellCapacitance.h "ym_cell/CellCapacitance.h"
/// @brief 容量値を表すクラス
//////////////////////////////////////////////////////////////////////
class CellCapacitance
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellCapacitance();

  /// @brief double からの変換コンストラクタ
  explicit
  CellCapacitance(double v);

  /// @brief デストラクタ
  ~CellCapacitance();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellCapacitance
  infty();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を取得する関数
  //////////////////////////////////////////////////////////////////////

  /// @brief 値を得る．
  double
  value() const;


public:
  //////////////////////////////////////////////////////////////////////
  // 代入演算子
  //////////////////////////////////////////////////////////////////////

  /// @brief 代入演算子
  const CellCapacitance&
  operator=(const CellCapacitance& src);

  /// @brief 加算付き代入演算子
  const CellCapacitance&
  operator+=(const CellCapacitance& src);

  /// @brief 減算付き代入演算子
  const CellCapacitance&
  operator-=(const CellCapacitance& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
CellCapacitance
operator+(const CellCapacitance& left,
	  const CellCapacitance& right);

/// @brief 減算
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
CellCapacitance
operator-(const CellCapacitance& left,
	  const CellCapacitance& right);

/// @brief 等価比較演算子
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
bool
operator==(const CellCapacitance& left,
	   const CellCapacitance& right);

/// @brief 非等価比較演算子
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
bool
operator!=(const CellCapacitance& left,
	   const CellCapacitance& right);

/// @brief 大小比較演算子
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
bool
operator<(const CellCapacitance& left,
	  const CellCapacitance& right);

/// @brief 大小比較演算子
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
bool
operator>(const CellCapacitance& left,
	  const CellCapacitance& right);

/// @brief 大小比較演算子
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
bool
operator<=(const CellCapacitance& left,
	   const CellCapacitance& right);

/// @brief 大小比較演算子
/// @paream[in] left, right オペランド
/// @relates CellCapacitance
bool
operator>=(const CellCapacitance& left,
	   const CellCapacitance& right);

/// @brief ストリーム出力
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @relates CellCapacitance
ostream&
operator<<(ostream& s,
	   const CellCapacitance& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////


// @brief 空のコンストラクタ
inline
CellCapacitance::CellCapacitance() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellCapacitance::CellCapacitance(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellCapacitance::~CellCapacitance()
{
}

// @brief 無限大の値を作る．
inline
CellCapacitance
CellCapacitance::infty()
{
  return CellCapacitance(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellCapacitance::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellCapacitance&
CellCapacitance::operator=(const CellCapacitance& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellCapacitance&
CellCapacitance::operator+=(const CellCapacitance& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellCapacitance&
CellCapacitance::operator-=(const CellCapacitance& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellCapacitance
operator+(const CellCapacitance& left,
	  const CellCapacitance& right)
{
  return CellCapacitance(left).operator+=(right);
}

// @brief 減算
inline
CellCapacitance
operator-(const CellCapacitance& left,
	  const CellCapacitance& right)
{
  return CellCapacitance(left).operator-=(right);
}

// @brief 等価比較演算子
// @paream[in] left, right オペランド
// @relates CellCapacitance
inline
bool
operator==(const CellCapacitance& left,
	   const CellCapacitance& right)
{
  return left.value() == right.value();
}

// @brief 非等価比較演算子
// @paream[in] left, right オペランド
// @relates CellCapacitance
inline
bool
operator!=(const CellCapacitance& left,
	   const CellCapacitance& right)
{
  return !operator==(left, right);
}

// @brief 大小比較演算子
// @paream[in] left, right オペランド
// @relates CellCapacitance
inline
bool
operator<(const CellCapacitance& left,
	  const CellCapacitance& right)
{
  return left.value() < right.value();
}

// @brief 大小比較演算子
// @paream[in] left, right オペランド
// @relates CellCapacitance
inline
bool
operator>(const CellCapacitance& left,
	  const CellCapacitance& right)
{
  return operator<(right, left);
}

// @brief 大小比較演算子
// @paream[in] left, right オペランド
// @relates CellCapacitance
inline
bool
operator<=(const CellCapacitance& left,
	   const CellCapacitance& right)
{
  return !operator<(right, left);
}

// @brief 大小比較演算子
// @paream[in] left, right オペランド
// @relates CellCapacitance
inline
bool
operator>=(const CellCapacitance& left,
	   const CellCapacitance& right)
{
  return !operator<(left, right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellCapacitance& val)
{
  return s << val.value();
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLCAPACITANCE_H
