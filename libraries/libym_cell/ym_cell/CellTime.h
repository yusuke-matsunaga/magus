#ifndef YM_CELL_CELLTIME_H
#define YM_CELL_CELLTIME_H

/// @file ym_cell/CellTime.h
/// @brief CellTime のヘッダファイル
/// @author Yusuke Matsunaga (松永 裕介)
///
/// Copyright (C) 2005-2011 Yusuke Matsunaga
/// All rights reserved.


#include "ym_cell/cell_nsdef.h"
#include "ym_utils/IDO.h"
#include "ym_utils/ODO.h"


BEGIN_NAMESPACE_YM_CELL

//////////////////////////////////////////////////////////////////////
/// @class CellTime CellTime.h "ym_cell/CellTime.h"
/// @brief 時間を表すクラス
//////////////////////////////////////////////////////////////////////
class CellTime
{
public:
  //////////////////////////////////////////////////////////////////////
  // コンストラクタ/デストラクタ
  //////////////////////////////////////////////////////////////////////

  /// @brief 空のコンストラクタ
  CellTime();

  /// @brief double からの変換用コンストラクタ
  explicit
  CellTime(double v);

  /// @brief デストラクタ
  ~CellTime();


public:
  //////////////////////////////////////////////////////////////////////
  // 値を作るクラスメソッド
  //////////////////////////////////////////////////////////////////////

  /// @brief 無限大の値を作る．
  static
  CellTime
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
  const CellTime&
  operator=(const CellTime& src);

  /// @brief 加算付き代入演算子
  const CellTime&
  operator+=(const CellTime& src);

  /// @brief 減算付き代入演算子
  const CellTime&
  operator-=(const CellTime& src);


private:
  //////////////////////////////////////////////////////////////////////
  // データメンバ
  //////////////////////////////////////////////////////////////////////

  // 値
  double mValue;

};

/// @brief 加算
/// @param[in] left, right オペランド
/// @relates CellTime
CellTime
operator+(const CellTime& left,
	  const CellTime& right);

/// @brief 減算
/// @param[in] left, right オペランド
/// @relates CellTime
CellTime
operator-(const CellTime& left,
	  const CellTime& right);

/// @brief 等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator==(const CellTime& left,
	   const CellTime& right);

/// @brief 非等価比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator!=(const CellTime& left,
	   const CellTime& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator<(const CellTime& left,
	  const CellTime& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator>(const CellTime& left,
	  const CellTime& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator<=(const CellTime& left,
	   const CellTime& right);

/// @brief 大小比較演算子
/// @param[in] left, right オペランド
/// @relates CellTime
bool
operator>=(const CellTime& left,
	   const CellTime& right);

/// @brief ストリーム出力
ostream&
operator<<(ostream& s,
	   const CellTime& val);

/// @brief バイナリダンプ
/// @param[in] s 出力先のストリーム
/// @param[in] val 値
/// @relates CellTime
ODO&
operator<<(ODO& s,
	   const CellTime& val);

/// @brief バイナリリストア
/// @param[in] s 入力元のストリーム
/// @param[out] val 値を格納する変数
/// @relates CellTime
IDO&
operator>>(IDO& s,
	   CellTime& val);


//////////////////////////////////////////////////////////////////////
// インライン関数の定義
//////////////////////////////////////////////////////////////////////

// @brief 空のコンストラクタ
inline
CellTime::CellTime() :
  mValue(0.0)
{
}

// @brief double からの変換コンストラクタ
inline
CellTime::CellTime(double v) :
  mValue(v)
{
}

// @brief デストラクタ
inline
CellTime::~CellTime()
{
}

// @brief 無限大の値を作る．
inline
CellTime
CellTime::infty()
{
  return CellTime(DBL_MAX);
}

// @brief 値を得る．
inline
double
CellTime::value() const
{
  return mValue;
}

// @brief 代入演算子
inline
const CellTime&
CellTime::operator=(const CellTime& src)
{
  mValue = src.mValue;
  return *this;
}

// @brief 加算付き代入演算子
inline
const CellTime&
CellTime::operator+=(const CellTime& src)
{
  mValue += src.mValue;
  return *this;
}

// @brief 減算付き代入演算子
inline
const CellTime&
CellTime::operator-=(const CellTime& src)
{
  mValue -= src.mValue;
  return *this;
}

// @brief 加算
inline
CellTime
operator+(const CellTime& left,
	  const CellTime& right)
{
  return CellTime(left).operator+=(right);
}

// @brief 減算
inline
CellTime
operator-(const CellTime& left,
	  const CellTime& right)
{
  return CellTime(left).operator-=(right);
}

// @brief 等価比較演算子
// @param[in] left, right オペランド
// @relates CellTime
inline
bool
operator==(const CellTime& left,
	   const CellTime& right)
{
  return left.value() == right.value();
}

// @brief 非等価比較演算子
// @param[in] left, right オペランド
// @relates CellTime
inline
bool
operator!=(const CellTime& left,
	   const CellTime& right)
{
  return !operator==(left, right);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellTime
inline
bool
operator<(const CellTime& left,
	  const CellTime& right)
{
  return left.value() < right.value();
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellTime
inline
bool
operator>(const CellTime& left,
	  const CellTime& right)
{
  return operator<(right, left);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellTime
inline
bool
operator<=(const CellTime& left,
	   const CellTime& right)
{
  return !operator<(right, left);
}

// @brief 大小比較演算子
// @param[in] left, right オペランド
// @relates CellTime
inline
bool
operator>=(const CellTime& left,
	   const CellTime& right)
{
  return !operator<(left, right);
}

// @brief ストリーム出力
inline
ostream&
operator<<(ostream& s,
	   const CellTime& val)
{
  return s << val.value();
}

// @brief バイナリダンプ
// @param[in] s 出力先のストリーム
// @param[in] val 値
// @relates CellTime
inline
ODO&
operator<<(ODO& s,
	   const CellTime& val)
{
  return s << val.value();
}

// @brief バイナリリストア
// @param[in] s 入力元のストリーム
// @param[out] val 値を格納する変数
// @relates CellTime
inline
IDO&
operator>>(IDO& s,
	   CellTime& val)
{
  double tmp_val;
  s >> tmp_val;
  val = CellTime(tmp_val);
  return s;
}

END_NAMESPACE_YM_CELL

#endif // YM_CELL_CELLTIME_H
