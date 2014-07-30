#ifndef CELL_CELLRESISTANCE_H
#define CELL_CELLRESISTANCE_H

/// @file YmCell/CellResistance.h
/// @brief CellResistance のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011, 2014 Yusuke Matsunaga
/// All rights reserved.


#include "YmCell/cell_nsdef.h"
#include "YmUtils/IDO.h"
#include "YmUtils/ODO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellResistance CellResistance.h "YmCell/CellResistance.h"
/// @brief 抵抗値を表すクラス
//////////////////////////////////////////////////////////////////////
class CellResistance
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellResistance();

  /// @brief double からの変換コンストラクタ
  explicit
  CellResistance(double v);

  /// @brief デストラクタ
  ~CellResistance();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellResistance
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
  const CellResistance&
  operator=(const CellResistance& src);

  /// @brief 加算付き代入演算子
  const CellResistance&
  operator+=(const CellResistance& src);

  /// @brief 減算付き代入演算子
  const CellResistance&
  operator-=(const CellResistance& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
/// @param[in] left, right オペランド
/// @relates CellResistance
CellResistance
operator+(const CellResistance& left,
	  const CellResistance& right);

/// @brief 減算
/// @param[in] left, right オペランド
/// @relates CellResistance
CellResistance
operator-(const CellResistance& left,
	  const CellResistance& right);

/// @brief 等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator==(const CellResistance& left,
	   const CellResistance& right);

/// @brief 非等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator!=(const CellResistance& left,
	   const CellResistance& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator<(const CellResistance& left,
	  const CellResistance& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator>(const CellResistance& left,
	  const CellResistance& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator<=(const CellResistance& left,
	   const CellResistance& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellResistance
bool
operator>=(const CellResistance& left,
	   const CellResistance& right);

/// @brief ストリーム出力
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @relates CellResistance
ostream&
operator<<(ostream& s,
	   const CellResistance& val);

/// @brief バイナリダンプ
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @relates CellResistance
ODO&
operator<<(ODO& s,
	   const CellResistance& val);

/// @brief バイナリリストア
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @relates CellResistance
IDO&
operator>>(IDO& s,
	   CellResistance& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
CellResistance::CellResistance() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellResistance::CellResistance(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellResistance::~CellResistance()
{
}

// @brief 無限大の値を作る．
inline
CellResistance
CellResistance::infty()
{
  return CellResistance(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellResistance::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellResistance&
CellResistance::operator=(const CellResistance& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellResistance&
CellResistance::operator+=(const CellResistance& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellResistance&
CellResistance::operator-=(const CellResistance& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellResistance
operator+(const CellResistance& left,
	  const CellResistance& right)
{
  return CellResistance(left).operator+=(right);
}

// @brief 減算
inline
CellResistance
operator-(const CellResistance& left,
	  const CellResistance& right)
{
  return CellResistance(left).operator-=(right);
}

// @brief 等価比較演算子
// @param[in] left, right オペランド
// @relates CellResistance
inline
bool
operator==(const CellResistance& left,
	   const CellResistance& right)
{
  return left.value() == right.value();
}

// @brief 非等価比較演算子
// @param[in] left, right オペランド
// @relates CellResistance
inline
bool
operator!=(const CellResistance& left,
	   const CellResistance& right)
{
  return !operator==(left, right);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellResistance
inline
bool
operator<(const CellResistance& left,
	  const CellResistance& right)
{
  return left.value() < right.value();
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellResistance
inline
bool
operator>(const CellResistance& left,
	  const CellResistance& right)
{
  return operator<(right, left);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellResistance
inline
bool
operator<=(const CellResistance& left,
	   const CellResistance& right)
{
  return !operator<(right, left);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellResistance
inline
bool
operator>=(const CellResistance& left,
	   const CellResistance& right)
{
  return !operator<(left, right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellResistance& val)
{
  return s << val.value();
}

// @brief バイナリダンプ
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @relates CellResistance
inline
ODO&
operator<<(ODO& s,
	   const CellResistance& val)
{
  return s << val.value();
}

// @brief バイナリリストア
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @relates CellResistance
inline
IDO&
operator>>(IDO& s,
	   CellResistance& val)
{
  double tmp_val;
  s >> tmp_val;
  val = CellResistance(tmp_val);
  return s;
}

END_NAMESPACE_YM_CELL

#endif // CELL_CELLRESISTANCE_H
